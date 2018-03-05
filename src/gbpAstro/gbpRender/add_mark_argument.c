#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gd.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpRender.h>

void add_mark_argument(render_info *render, const char *species, int value, const char *type, ...) {
    va_list vargs;
    va_start(vargs, type);

    // Create new argument
    mark_arg_info *new_arg = NULL;
    create_mark_argument(render, &new_arg);
    strcpy(new_arg->species, species);
    new_arg->value                = (char)value;
    new_arg->flag_keep_properties = GBP_FALSE;
    strcpy(new_arg->type, type);

    // Parse the passed parameters
    if(!strcmp(new_arg->type, "sphere")) {
        for(int i_val = 0; i_val < 4; i_val++)
            new_arg->dval[i_val] = va_arg(vargs, double);
    } else if(!strcmp(new_arg->type, "group_index") || !strcmp(new_arg->type, "subgroup_index") || !strcmp(new_arg->type, "group_id") ||
              !strcmp(new_arg->type, "subgroup_id")) {
        for(int i_val = 0; i_val < 1; i_val++)
            new_arg->ival[i_val] = va_arg(vargs, int);
        new_arg->flag_keep_properties = GBP_TRUE;
    } else if(!strcmp(new_arg->type, "group_tree_id") || !strcmp(new_arg->type, "subgroup_tree_id")) {
        for(int i_val = 0; i_val < 1; i_val++)
            new_arg->ival[i_val] = va_arg(vargs, int);
    } else if(!strcmp(new_arg->type, "<n_p")) {
        for(int i_val = 0; i_val < 1; i_val++)
            new_arg->ival[i_val] = va_arg(vargs, int);
    } else if(!strcmp(new_arg->type, "") || !strcmp(new_arg->type, "*"))
        sprintf(new_arg->type, "all");

    // These things don't take argument values
    else if(strcmp(new_arg->type, "group_fragmented") && strcmp(new_arg->type, "subgroup_fragmented"))
        SID_exit_error("Invalid mark type {%s} in add_mark_argument().", SID_ERROR_LOGIC, new_arg->type);

    // Set first/last/last->next pointers and increment counter
    if(render->mark_arg_first == NULL)
        render->mark_arg_first = new_arg;
    if(render->mark_arg_last != NULL)
        render->mark_arg_last->next = new_arg;
    render->mark_arg_last = new_arg;
    render->n_mark_args++;

    // Count the number of arguments that need to keep properties
    if(new_arg->flag_keep_properties)
        render->n_mark_properties++;

    va_end(vargs);
}
