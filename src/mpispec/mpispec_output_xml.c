/*
 *  cspec_output_xml.c   :  Xml output
 *
 * See copyright notice in cspec.h
 *
 */

/*
 * MPISpec doesn't contain cspec.h. please see mpispec.h.
 */

#include <mpi.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mpispec_basic.h"
#include "mpispec_comm_world.h"
#include "mpispec_error.h"
#include "mpispec_error_function.h"
#include "mpispec_output_xml.h"
#include "mpispec_util.h"

#define XML_SUMMARY_FILE_NAME "output.xml"

static MPISpecOutputStruct xml;
static FILE *output_xml_file = NULL;
static int mpispec_tab_num = 0;

static void merge_xml_file(void);

static void start_describe_fun_xml(const char *descr);
static void end_describe_fun_xml(void);
static void start_it_fun_xml(const char *descr);
static void end_it_fun_xml(void);
static void eval_fun_xml(const char *filename, int line_number,
                         const char *assertion, int assertion_result);
static void pending_fun_xml(const char *reason);

static void fprint_tab(int n);
static void get_xml_file_name(char *xml_filename, const char *filename);
static void get_rank_xml_file_name(char *xml_filename, const char *filename,
                                   int rank);
static void write_behavior(FILE *fp, const char *encoding);
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
}

void MPISpec_XmlFileClose(void) {
    if (output_xml_file != NULL) {
        mpispec_tab_num--;
        fprint_tab(mpispec_tab_num);
        fclose(output_xml_file);
    }
    MPI_Barrier(MPISPEC_COMM_WORLD);
    if (MPISpec_Rank() == 0) merge_xml_file();
}

void merge_xml_file(void) {
    int i, ch, size = MPISpec_Size();
    char xml_filename[MPISPEC_MAX_XML_FILENAME_LEN];
    FILE *fp, *summary;
    remove(MPISPEC_XML_BASE_FILENAME);
    summary = fopen(MPISPEC_XML_BASE_FILENAME, "a");
    write_behavior(summary, MPISPEC_XML_ENCODING);
    for (i = 0; i < size; i++) {
        get_rank_xml_file_name(xml_filename, MPISPEC_XML_BASE_FILENAME, i);
        if (NULL == (fp = fopen(xml_filename, "r"))) {
            continue;
        }
        while ((ch = fgetc(fp)) != EOF) {
            fputc(ch, summary);
        }
        fclose(fp);
        remove(xml_filename);
    }
    fprintf(summary, "</BEHAVIOUR>\n");
    fclose(summary);
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

void fprint_tab(int n) {
    int i;
    for (i = 0; i < n; i++) fprintf(output_xml_file, MPISPEC_TAB);
}

void get_xml_file_name(char *xml_filename, const char *filename) {
    get_rank_xml_file_name(xml_filename, filename, MPISpec_Rank());
}

void get_rank_xml_file_name(char *xml_filename, const char *filename,
                            int rank) {
    sprintf(xml_filename, "rank%04d_%s", rank, filename);
}

void write_behavior(FILE *fp, const char *encoding) {
    time_t time_value;
    char *time_str;

    time(&time_value);
    time_str = ctime(&time_value);
    time_str[strlen(time_str) - 1] = '\0';

    fprintf(fp, "<?xml version=\"1.0\" encoding=\"%s\" ?>\n", encoding);
    fprintf(fp,
            "<?xml-stylesheet type=\"text/xsl\" href=\"MPISpec-Run.xsl\" ?>\n");
    fprintf(fp, "<BEHAVIOUR timestamp=\"%s\">\n", time_str);
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
