/*
 *  MPISpec - A Behavior Driven Development Framework for MPI Programs, based on CUnit, CSpec and MPIUnit.
 *
 *  License:    LGPL
 *  Author:     Yuu Shigetani
 *  Time-stamp: 2014/01/17 06:02:29
 */

/*
 *  cspec_output_junit_xml.c   :  JUnit Xml output
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. So, please see mpispec.h.
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "mpispec_output_junit_xml.h"
#include "mpispec_private_output_junit_xml.h"
#include "mpispec_consts.h"

#define DESCRIBE_NUM 10
#define IT_NUM 10

static MPISpecOutputStruct xml;
static FILE *output_xml_file = NULL;
static int desc_outputs_num;
static MPISpecDescOutputStruct *desc_outputs;

static const char* const failure_message = "Failed";
static const char* const failure_type = "";

static char *desc_name_prefix_array[MPISPEC_MAX_NEST_NUM];

static void desc_name(char **name, char *descr);
static void desc_prefix_name(char **prefix);
static void push_desc_prefix_name(char *descr);
static void pop_desc_prefix_name(void);

static void start_describe_fun_junit_xml(const char *descr);
static int start_describe_fun_junit_xml_expand_if_needed(void);
static int start_describe_fun_junit_xml_init_descr(MPISpecDescOutputStruct* const target_descr, const char *descr);
static void end_describe_fun_junit_xml(void);
static void start_it_fun_junit_xml(const char *descr);
static int start_it_fun_junit_xml_expand_if_needed(MPISpecDescOutputStruct* const target_descr);
static int start_it_fun_junit_xml_init_it(MPISpecDescOutputStruct* const target_descr, const char* const descr);
static int start_it_fun_junit_xml_set_descr(MPISpecItOutputStruct* const target_it, const char* const descr);
static int start_it_fun_junit_xml_set_failure(MPISpecItOutputStruct* const target_it);
static void end_it_fun_junit_xml(void);
static void eval_fun_junit_xml(const char *filename, int line_number, const char *assertion, int assertionResult);
static void pending_fun_junit_xml(const char *reason);

static void output_header(const char *encoding);
static void output_footer(void);
static void output_describe(void);
static void output_describe_header(const MPISpecDescOutputStruct* const descr);
static void output_describe_main(const MPISpecDescOutputStruct* const descr);
static void output_describe_footer(void);
static void output_it(const MPISpecItOutputStruct* const it);
static void output_it_header(const MPISpecItOutputStruct* const it);
static void output_it_main(const MPISpecItOutputStruct* const it);
static void output_it_footer(void);
static int sumup_failure(const MPISpecDescOutputStruct* const p);
static void destruct(void);
static void destruct_descr(MPISpecDescOutputStruct* const descr);
static void destruct_it(MPISpecItOutputStruct* const it);
static void xml_file_close(void);
static int is_failure_struct(const MPISpecFailureStruct* const fail);
static void test_fails(const char *filename, int line_number, const char *assertion);
static void add_failure(MPISpecFailureStruct *failure);


void
MPISpec_JUnitXmlFileOpen(const char *filename, const char *encoding)
{
    if (output_xml_file != NULL)
        return;

    char xml_filename[MPISPEC_MAX_XML_FILENAME_LEN];
    int myrank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    sprintf(xml_filename, "rank%d_%s", myrank, filename);
    output_xml_file = fopen(xml_filename, "w");

    if (output_xml_file == NULL)
        return;

    desc_outputs_num = 0;
    desc_outputs = NULL;

    output_header(encoding);
}

void
MPISpec_JUnitXmlFileClose(void)
{
    if (output_xml_file == NULL)
        return;

    output_describe();
    output_footer();

    destruct();

    xml_file_close();
}

void
output_header(const char *encoding)
{
    fprintf(output_xml_file, "<?xml version=\"1.0\" encoding=\"%s\" ?>\n", encoding);
    fprintf(output_xml_file, "<testsuites>\n");
}

void
output_footer(void)
{
    fprintf(output_xml_file, "</testsuites>\n");
}

void
output_describe(void)
{
    int i;
    for (i = 0; i < desc_outputs_num; ++i) {
        const MPISpecDescOutputStruct* const descr = desc_outputs + i;
        if (descr->it_outputs == 0)
            continue;

        output_describe_header(descr);
        output_describe_main(descr);
        output_describe_footer();
    }
}

void
output_describe_header(const MPISpecDescOutputStruct* const descr)
{
    int n_failure = sumup_failure(descr);
    fprintf(output_xml_file,
            "  <testsuite errors=\"0\" failures=\"%d\" name=\"%s\" tests=\"%d\">\n",
            n_failure,
            descr->descr,
            descr->it_output_num);
}

void
output_describe_main(const MPISpecDescOutputStruct* const descr)
{
    int j;
    for (j = 0; j < descr->it_output_num; ++j)
        output_it(descr->it_outputs + j);
}

void
output_describe_footer(void)
{
    fprintf(output_xml_file, "  </testsuite>\n");
}

int
sumup_failure(const MPISpecDescOutputStruct* const descr)
{
    int j;
    int sum = 0;
    for (j = 0; j < descr->it_output_num; ++j)
        sum += descr->it_outputs[j].failures->size;
    return sum;
}

void
output_it(const MPISpecItOutputStruct* const it)
{
    output_it_header(it);
    output_it_main(it);
    output_it_footer();
}

void
output_it_header(const MPISpecItOutputStruct* const it)
{
    fprintf(output_xml_file,
            "    <testcase name=\"%s\" assertions=\"%d\">\n",
            it->descr,
            it->assert_num - it->pending_num);
}

void
output_it_main(const MPISpecItOutputStruct* const it)
{
    size_t k;

    for (k = 0; k < it->failures->size; ++k) {
        const MPISpecFailureStruct* const fail = array_get_element(it->failures, k);
        if (is_failure_struct(fail) < 0) return;

        fprintf(output_xml_file,
                "      <failure message=\"%s\" type=\"%s\">\n",
                fail->message,
                fail->type);
        fprintf(output_xml_file,
                "%s:%d: %s\n",
                fail->fname,
                fail->line,
                fail->assertion_descr);
        fprintf(output_xml_file, "      </failure>\n");
    }
}

void
output_it_footer(void)
{
    fprintf(output_xml_file, "    </testcase>\n");
}

void
destruct(void)
{
    int i;
    for (i = 0; i < desc_outputs_num; ++i)
        destruct_descr(desc_outputs + i);
    free(desc_outputs);
    desc_outputs = NULL;
}

void
destruct_descr(MPISpecDescOutputStruct* const descr)
{
    if (NULL == descr)
        return;

    int j;
    if (NULL != descr->descr) {
        free(descr->descr);
        descr->descr = NULL;
    }
    if (NULL != descr->it_outputs) {
        for (j = 0; j < descr->it_output_num; ++j)
            destruct_it(descr->it_outputs + j);
        free(descr->it_outputs);
        descr->it_outputs = NULL;
    }
    descr->it_output_num = 0;
}

void
destruct_it(MPISpecItOutputStruct* const it)
{
    if (NULL == it)
        return;
    if (NULL != it->descr) {
        free(it->descr);
        it->descr = NULL;
    }
    array_delete(&(it->failures));
}

void
xml_file_close(void)
{
    int ret = fclose(output_xml_file);
    if (0 != ret)
        fprintf(stderr, "[ERR] %s(%d) fclose() failed\n", __FILE__, __LINE__);
    output_xml_file = NULL;
}

void
start_describe_fun_junit_xml(const char *descr)
{
    if (output_xml_file == NULL)
        return;

    int ret;
    ret = start_describe_fun_junit_xml_expand_if_needed();
    if (0 != ret)
        return;

    ret = start_describe_fun_junit_xml_init_descr(desc_outputs + desc_outputs_num, descr);
    if (0 != ret)
        return;

    ++desc_outputs_num;
}

int
start_describe_fun_junit_xml_expand_if_needed(void)
{
    if (0 == (desc_outputs_num % DESCRIBE_NUM)) {
        MPISpecDescOutputStruct* p = realloc(desc_outputs, (desc_outputs_num + DESCRIBE_NUM) * sizeof(MPISpecDescOutputStruct));
        if (NULL == p) {
            fprintf(stderr, "[ERR] %s(%d) realloc(%d * %d) failed\n", __FILE__, __LINE__,
                    desc_outputs_num + DESCRIBE_NUM,
                    (int) sizeof(MPISpecDescOutputStruct));
            destruct();
            xml_file_close();
            return -1;
        }
        desc_outputs = p;
    }
    return 0;
}

int
start_describe_fun_junit_xml_init_descr(MPISpecDescOutputStruct* const target_descr, const char *descr)
{
    char *name = "";
    char *d    = strdup(descr);

    desc_name(&name, d);

    target_descr->descr       = name;
    target_descr->it_output_num = 0;
    target_descr->it_outputs   = NULL;
    push_desc_prefix_name(d);
    return 0;
}

void
end_describe_fun_junit_xml(void)
{
    pop_desc_prefix_name();
}

void
start_it_fun_junit_xml(const char *descr)
{
    if (output_xml_file == NULL) return;

    MPISpecDescOutputStruct *target_descr;
    int ret;

    target_descr = desc_outputs + (desc_outputs_num - 1);
    ret = start_it_fun_junit_xml_expand_if_needed(target_descr);
    if (0 != ret) return;

    ret = start_it_fun_junit_xml_init_it(target_descr, descr);
    if (0 != ret) return;

    ++(target_descr->it_output_num);
}

int
start_it_fun_junit_xml_expand_if_needed(MPISpecDescOutputStruct* const target_descr)
{
    if (0 == (target_descr->it_output_num % IT_NUM)) {
        MPISpecItOutputStruct *p = realloc(target_descr->it_outputs,
                (target_descr->it_output_num + IT_NUM) * sizeof(MPISpecItOutputStruct));
        if (NULL == p) {
            fprintf(stderr, "[ERR] %s(%d) realloc(%d * %d) failed\n", __FILE__, __LINE__,
                    target_descr->it_output_num + IT_NUM,
                    (int) sizeof(MPISpecItOutputStruct));
            destruct();
            xml_file_close();
            return -1;
        }
        target_descr->it_outputs = p;
    }
    return 0;
}

int
start_it_fun_junit_xml_init_it(MPISpecDescOutputStruct* const target_descr, const char* const descr)
{
    int ret;
    MPISpecItOutputStruct *target_it = target_descr->it_outputs + target_descr->it_output_num;

    target_it->assert_num = 0;
    target_it->pending_num = 0;
    ret = start_it_fun_junit_xml_set_descr(target_it, descr);
    if (0 != ret) return -1;
    ret = start_it_fun_junit_xml_set_failure(target_it);
    if (0 != ret) return -2;
    return 0;
}

int
start_it_fun_junit_xml_set_descr(MPISpecItOutputStruct* const target_it, const char* const descr)
{
    target_it->descr = strdup(descr);
    if (NULL == target_it->descr) {
        fprintf(stderr, "[ERR] %s(%d) strdup(%p) failed\n", __FILE__, __LINE__, descr);
        destruct();
        xml_file_close();
        return -1;
    }
    return 0;
}

int
start_it_fun_junit_xml_set_failure(MPISpecItOutputStruct* const target_it)
{
    target_it->failures = array_new(sizeof(MPISpecFailureStruct));
    if (NULL == target_it->failures) {
        fprintf(stderr, "[ERR] %s(%d) array_new(%d) failed\n", __FILE__, __LINE__, (int) sizeof(MPISpecFailureStruct));
        destruct();
        xml_file_close();
        return -1;
    }
    return 0;
}

void
end_it_fun_junit_xml(void)
{
}

void
eval_fun_junit_xml(const char *filename, int line_number, const char *assertion, int assertion_result)
{
    if (output_xml_file == NULL) return;

    ++(desc_outputs[desc_outputs_num - 1].it_outputs[desc_outputs[desc_outputs_num - 1].it_output_num - 1].assert_num);

    if (! assertion_result)
        test_fails(filename, line_number, assertion);
}

void
pending_fun_junit_xml(const char *reason)
{
    if (output_xml_file == NULL) return;
    ++(desc_outputs[desc_outputs_num - 1].it_outputs[desc_outputs[desc_outputs_num - 1].it_output_num - 1].pending_num);
}

MPISpecOutputStruct*
MPISpec_NewOutputJUnitXml(void)
{
    MPISpec_InitOutput(&xml);

    xml.start_describe_fun = start_describe_fun_junit_xml;
    xml.end_describe_fun   = end_describe_fun_junit_xml;
    xml.start_it_fun       = start_it_fun_junit_xml;
    xml.end_it_fun         = end_it_fun_junit_xml;
    xml.eval_fun           = eval_fun_junit_xml;
    xml.pending_fun        = pending_fun_junit_xml;

    return &xml;
}

static void
desc_name(char **name, char *descr)
{
    char *prefix = "";
    desc_prefix_name(&prefix);
    // [TODO] - remove asprintf
    if (asprintf(name, "%s %s", prefix, descr) < 0) {
        printf("Couldn't combine prefix & description names.\n");
        exit(-1);
    }
}

static void
desc_prefix_name(char **prefix)
{
    int i;
    for(i = 0; i < MPISPEC_MAX_NEST_NUM; i++) {
        if (desc_name_prefix_array[i] != NULL) {
            // [TODO] - remove asprintf
            if (asprintf(prefix, "%s %s", *prefix, desc_name_prefix_array[i]) < 0) {
                printf("Couldn't combine prefix & description names.\n");
                exit(-1);
            }
        }
        else {
            break;
        }
    }
}

static void
push_desc_prefix_name(char *descr)
{
    int i;
    for (i = 0; i < MPISPEC_MAX_NEST_NUM; i++) {
        if (desc_name_prefix_array[i] == NULL) {
            desc_name_prefix_array[i] = descr;
            break;
        }
    }
}

static void
pop_desc_prefix_name(void)
{
    int i;
    for (i = 1; i <= MPISPEC_MAX_NEST_NUM; i++)
        if (desc_name_prefix_array[i] == NULL)
            break;
    if (desc_name_prefix_array[i - 1] != NULL)
        desc_name_prefix_array[i - 1] = NULL;
}

int
is_failure_struct(const MPISpecFailureStruct* const fail)
{
    if (fail != NULL) return 0;
    fprintf(stderr,
            "[ERR] %s(%d) array_get_element returns NULL\n",
            __FILE__,
            __LINE__);
    destruct();
    xml_file_close();
    return -1;
}

void
test_fails(const char *filename, int line_number, const char *assertion)
{
    MPISpecFailureStruct failure;

    failure.message         = failure_message;
    failure.type            = failure_type;
    failure.fname           = filename;
    failure.line            = line_number;
    failure.assertion_descr = assertion;
    add_failure(&failure);
}

void
add_failure(MPISpecFailureStruct *failure)
{
    int ret;
    ret = array_add(desc_outputs[desc_outputs_num - 1].it_outputs[desc_outputs[desc_outputs_num - 1].it_output_num - 1].failures, failure);
    if (ret == 0) return;
    fprintf(stderr,
            "[ERR] %s(%d) array_add() failed (ret=%d,desc_outputs=%p,desc_outputs_num=%d,it_outputs=%p,it_output_num=%d,failures=%p)\n",
            __FILE__,
            __LINE__,
            ret,
            desc_outputs,
            desc_outputs_num,
            desc_outputs[desc_outputs_num - 1].it_outputs,
            desc_outputs[desc_outputs_num - 1].it_output_num,
            desc_outputs[desc_outputs_num - 1].it_outputs[desc_outputs[desc_outputs_num - 1].it_output_num - 1].failures);
    destruct();
    xml_file_close();
}
