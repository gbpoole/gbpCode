#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <gbpSID.h>
#include <gbpADaPS.h>

void *ADaPS_fetch(ADaPS *list, const char *name_in, ...) {
    ADaPS * current;
    int     flag;
    void *  r_val;
    va_list vargs;
    char    name[ADaPS_NAME_LENGTH];
    va_start(vargs, name_in);
    vsprintf(name, name_in, vargs);
    current = list;
    r_val   = NULL;
    flag    = GBP_TRUE;
    while(current != NULL && flag) {
        if(!strcmp(name, current->name)) {
            r_val = current->data;
            flag  = GBP_FALSE;
        }
        current = current->next;
    }
    if(flag)
        SID_exit_error("Variable {%s} was not found in ADaPS structure.", SID_ERROR_LOGIC, name);
    va_end(vargs);
    return (r_val);
}
