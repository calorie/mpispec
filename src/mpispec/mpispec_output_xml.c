/*
 *  cspec_output_xml.c   :  Xml output
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. So, please see mpispec.h.
 */

#include <mpi.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "mpispec_consts.h"
#include "mpispec_output_xml.h"

static MPISpecOutputStruct xml;
static FILE *output_xml_file = NULL;
static int mpispec_tab_num = 0;

static void fprint_tab(int n);
static void get_xml_file_name(char *xml_filename, const char *filename);
static void write_behavior(const char *encoding);
static void write_result(const char *filename, int line_number,
                         const char *assertion, int assertion_result);
static void test_success(const char *assertion);
static void test_fails(const char *filename, int line_number,
                       const char *assertion);

void MPISpec_XmlFileOpen(const char *filename, const char *encoding) {
    if (output_xml_file != NULL) return;

    char xml_filename[MPISPEC_MAX_XML_FILENAME_LEN];
    get_xml_file_name(xml_filename, filename);

    output_xml_file = fopen(xml_filename, "w");
    if (output_xml_file == NULL) return;

    write_behavior(encoding);
}

void MPISpec_XmlFileClose(void) {
    if (output_xml_file == NULL) return;

    mpispec_tab_num--;
    fprint_tab(mpispec_tab_num);
    fprintf(output_xml_file, "</BEHAVIOUR>\n");

    fclose(output_xml_file);
}

void start_describe_fun_xml(const char *descr) {
    if (output_xml_file == NULL) return;

    fprint_tab(mpispec_tab_num);
    fprintf(output_xml_file, "<DESCRIBE>\n");
    fprint_tab(mpispec_tab_num + 1);
    fprintf(output_xml_file, "<DESCRIPTION><![CDATA[%s]]></DESCRIPTION>\n",
            descr);
    mpispec_tab_num++;
}

void end_describe_fun_xml(void) {
    if (output_xml_file == NULL) return;

    mpispec_tab_num--;
    fprint_tab(mpispec_tab_num);
    fprintf(output_xml_file, "</DESCRIBE>\n");
}

void start_it_fun_xml(const char *descr) {
    if (output_xml_file == NULL) return;

    fprint_tab(mpispec_tab_num);
    fprintf(output_xml_file, "<IT>\n");
    fprint_tab(mpispec_tab_num + 1);
    fprintf(output_xml_file, "<DESCRIPTION><![CDATA[it %s]]></DESCRIPTION>\n",
            descr);
}

void end_it_fun_xml(void) {
    if (output_xml_file == NULL) return;

    fprint_tab(mpispec_tab_num);
    fprintf(output_xml_file, "</IT>\n");
}

void eval_fun_xml(const char *filename, int line_number, const char *assertion,
                  int assertion_result) {
    if (output_xml_file == NULL) return;

    mpispec_tab_num++;
    fprint_tab(mpispec_tab_num);
    fprintf(output_xml_file, "<ASSERTION>\n");
    fprint_tab(mpispec_tab_num + 1);

    write_result(filename, line_number, assertion, assertion_result);

    fprint_tab(mpispec_tab_num);
    fprintf(output_xml_file, "</ASSERTION>\n");
    mpispec_tab_num--;
}

void pending_fun_xml(const char *reason) {
    if (output_xml_file == NULL) return;

    mpispec_tab_num++;
    fprint_tab(mpispec_tab_num);
    fprintf(output_xml_file, "<ASSERTION>\n");

    fprint_tab(mpispec_tab_num + 1);
    fprintf(output_xml_file, "<RESULT>Pending</RESULT>\n");
    fprint_tab(mpispec_tab_num + 1);
    fprintf(output_xml_file, "<MESSAGE><![CDATA[%s]]></MESSAGE>\n", reason);

    fprint_tab(mpispec_tab_num);
    fprintf(output_xml_file, "</ASSERTION>\n");
}

MPISpecOutputStruct *MPISpec_NewOutputXml(void) {
    MPISpec_InitOutput(&xml);

    xml.start_describe_fun = start_describe_fun_xml;
    xml.end_describe_fun = end_describe_fun_xml;
    xml.start_it_fun = start_it_fun_xml;
    xml.end_it_fun = end_it_fun_xml;
    xml.eval_fun = eval_fun_xml;
    xml.pending_fun = pending_fun_xml;

    return &xml;
}

void get_xml_file_name(char *xml_filename, const char *filename) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    sprintf(xml_filename, "rank%d_%s", rank, filename);
}

void write_behavior(const char *encoding) {
    time_t time_value;
    char *time_str;

    time(&time_value);
    time_str = ctime(&time_value);
    time_str[strlen(time_str) - 1] = '\0';

    fprintf(output_xml_file, "<?xml version=\"1.0\" encoding=\"%s\" ?>\n",
            encoding);
    fprintf(output_xml_file,
            "<?xml-stylesheet type=\"text/xsl\" href=\"MPISpec-Run.xsl\" ?>\n");
    fprintf(output_xml_file, "<BEHAVIOUR timestump=\"%s\">\n", time_str);
    mpispec_tab_num++;
}

void fprint_tab(int n) {
    int i;
    for (i = 0; i < n; i++) fprintf(output_xml_file, MPISPEC_TAB);
}

void write_result(const char *filename, int line_number, const char *assertion,
                  int assertion_result) {
    if (assertion_result)
        test_success(assertion);
    else
        test_fails(filename, line_number, assertion);
}

void test_success(const char *assertion) {
    fprintf(output_xml_file, "<RESULT>OK</RESULT>\n");
    fprint_tab(mpispec_tab_num + 1);
    fprintf(output_xml_file, "<MESSAGE><![CDATA[%s]]></MESSAGE>\n", assertion);
}

void test_fails(const char *filename, int line_number, const char *assertion) {
    fprintf(output_xml_file, "<RESULT>Failure</RESULT>\n");
    fprint_tab(mpispec_tab_num + 1);
    fprintf(output_xml_file,
            "<MESSAGE><![CDATA[%s in file %s at line %d]]></MESSAGE>\n",
            assertion, filename, line_number);
}
