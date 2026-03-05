#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int loops = 0;
int decisions = 0;
int depth = 0;
int max_depth = 0;
int loop_depth = 0;
int max_loop_depth = 0;
int function_count = 0;
int recursion_detected = 0;
int cyclomatic_complexity = 1;
int has_division = 0;
int is_binary_search = 0;
int open_loops = 0;  // Track active loop nesting
int has_loops_and_recursion = 0;  // For detecting merge sort pattern

char current_function[100] = "";
char functions[100][100];
int func_idx = 0;

// Helper to extract function name properly
int extract_function_name(char *line, char *name) {
    char *paren = strchr(line, '(');
    if (!paren) return 0;
    
    // Go backwards from '(' to find the function name
    int i = paren - line - 1;
    int j = 0;
    
    // Skip whitespace before (
    while (i >= 0 && isspace(line[i])) i--;
    
    // Extract the identifier backwards
    while (i >= 0 && (isalnum(line[i]) || line[i] == '_')) {
        i--;
    }
    i++; // Move back to the start of the identifier
    
    // Now extract the name
    while (line[i] && (isalnum(line[i]) || line[i] == '_') && line[i] != '(') {
        name[j++] = line[i++];
    }
    name[j] = '\0';
    
    return (j > 0) ? 1 : 0;
}

// Check if a function is defined (has opening brace on same or next analysis)
int is_function_definition(char *line) {
    // Look for pattern: identifier ( ... ) {
    char *paren_open = strchr(line, '(');
    char *paren_close = strchr(line, ')');
    char *brace = strchr(line, '{');
    
    if(paren_open && paren_close && brace && paren_open < paren_close && paren_close < brace) {
        return 1;
    }
    return 0;
}

// Remove comments from line
void remove_comments(char *line) {
    char *comment = strstr(line, "//");
    if(comment) {
        *comment = '\0';
    }
}

void analyze_line(char *line){
    remove_comments(line);
    
    // Count loops (handle both "while(" and "while (" patterns)
    if(strstr(line,"for(") || strstr(line,"for (") || strstr(line,"while(") || strstr(line,"while (") || strstr(line,"do {") || strstr(line,"do{"))
    {
        loops++;
        open_loops++;
        loop_depth = open_loops;
        if(loop_depth > max_loop_depth)
            max_loop_depth = loop_depth;
        // DO NOT count loops as decisions - they are separate metrics
        cyclomatic_complexity++;  // But loops DO count toward cyclomatic complexity
    }

    // Count decisions (if, else-if, switch, ternary) - handle spaces
    if(strstr(line,"if(") || strstr(line,"if (") || strstr(line,"else if(") || strstr(line,"else if ("))
    {
        decisions++;
        cyclomatic_complexity++;
    }
    
    // Count standalone else statements (that start the line, not "} else")
    // This catches "else high = mid - 1;" but not "} else {"
    char *else_pos = strstr(line, "else");
    if(else_pos && !strstr(line,"else if") && !strstr(line,"} else"))
    {
        // Find start of line (skip whitespace)
        int is_line_start = 1;
        for(char *p = line; p < else_pos; p++) {
            if(!isspace(*p)) {
                is_line_start = 0;
                break;
            }
        }
        if(is_line_start) {
            decisions++;
            // DO NOT increment cyclomatic_complexity - else is alternative path
        }
    }
    
    if(strstr(line,"switch(") || strstr(line,"switch ("))
    {
        decisions++;
        cyclomatic_complexity++;
    }
    
    if(strstr(line,"case ") || strstr(line,"default:"))
    {
        decisions++;
        cyclomatic_complexity++;
    }
    
    // Ternary operator (? :)
    if(strchr(line, '?') && strchr(line, ':'))
    {
        decisions++;
        cyclomatic_complexity++;
    }

    // Track depth based on braces (for general nesting info)
    int i = 0;
    while(line[i]) {
        if(line[i] == '{') {
            depth++;
            if(depth > max_depth)
                max_depth = depth;
        }
        else if(line[i] == '}') {
            if(depth > 0)
                depth--;
            if(open_loops > 0)
                open_loops--;
        }
        i++;
    }
    
    // Update loop_depth based on current open loops
    loop_depth = open_loops;

    // Detect binary search pattern (division in loop)
    if(loops > 0 && (strchr(line, '/') || strstr(line, ">>"))) {
        has_division = 1;
    }
    
    // Detect binary search specific patterns
    if(strstr(line, "low") || strstr(line, "high") || strstr(line, "mid")) {
        is_binary_search = 1;
    }

    // Function definition detection (improved)
    if(is_function_definition(line))
    {
        char name[100];
        if(extract_function_name(line, name))
        {
            if(strcmp(name,"if")!=0 &&
               strcmp(name,"for")!=0 &&
               strcmp(name,"while")!=0 &&
               strcmp(name,"switch")!=0 &&
               strcmp(name,"else")!=0)
            {
                // Only count unique functions
                int is_new = 1;
                for(int i = 0; i < func_idx; i++) {
                    if(strcmp(functions[i], name) == 0) {
                        is_new = 0;
                        break;
                    }
                }
                
                if(is_new) {
                    strcpy(functions[func_idx], name);
                    func_idx++;
                    function_count++;
                }
                
                strcpy(current_function, name);
            }
        }
    }

    // Recursion detection (improved - only if function calls itself)
    if(strlen(current_function) > 0 && strstr(line, current_function) && 
       strstr(line,"(") && strstr(line,")") && !strstr(line,"{"))
    {
        // Make sure it's not the definition line
        if(!strstr(line,"void ") && !strstr(line,"int ") && !strstr(line,"char ") &&
           !strstr(line,"float ") && !strstr(line,"double ") && !strstr(line,"struct "))
        {
            recursion_detected = 1;
        }
    }
}

int main(int argc, char **argv){

    if(argc < 2){
        printf("Usage: analyzer <file.c>\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if(!file){
        printf("Cannot open file\n");
        return 1;
    }

    char line[1024];
    while(fgets(line, sizeof(line), file) != NULL) {
        analyze_line(line);
    }

    fclose(file);

    char complexity[20];
    
    // Detect merge sort pattern (recursion + loops + division)
    if(recursion_detected && loops > 0 && has_division) {
        has_loops_and_recursion = 1;
    }

    // Improved complexity calculation
    if(has_loops_and_recursion)
    {
        // Merge sort pattern: recursion + loops + division = O(n log n)
        strcpy(complexity, "O(n log n)");
    }
    else if(recursion_detected && max_loop_depth == 0)
    {
        // Recursion without loops - could be exponential or linear
        strcpy(complexity, "O(2^n)");
    }
    else if(recursion_detected && max_loop_depth >= 1)
    {
        // Recursion with loops but not divide-and-conquer pattern
        strcpy(complexity, "O(n*2^n)");
    }
    else if(max_loop_depth == 0)
        strcpy(complexity, "O(1)");
    else if(max_loop_depth == 1)
    {
        // Check for binary search pattern (division with low/high/mid)
        if(has_division && is_binary_search)
            strcpy(complexity, "O(log n)");
        else
            strcpy(complexity, "O(n)");
    }
    else if(max_loop_depth == 2)
        strcpy(complexity, "O(n^2)");
    else if(max_loop_depth == 3)
        strcpy(complexity, "O(n^3)");
    else
        strcpy(complexity, "O(n^k)");

    FILE *out = fopen("result.json","w");

    fprintf(out,"{\n");
    fprintf(out,"  \"functions\": %d,\n", function_count);
    fprintf(out,"  \"recursion\": %d,\n", recursion_detected);
    fprintf(out,"  \"loops\": %d,\n", loops);
    fprintf(out,"  \"decisions\": %d,\n", decisions);
    fprintf(out,"  \"max_depth\": %d,\n", max_depth);
    fprintf(out,"  \"max_loop_depth\": %d,\n", max_loop_depth);
    fprintf(out,"  \"cyclomatic_complexity\": %d,\n", cyclomatic_complexity);
    fprintf(out,"  \"complexity\": \"%s\"\n", complexity);
    fprintf(out,"}\n");

    fclose(out);

    printf("Analysis complete. Results saved to result.json\n");
    printf("Complexity: %s | Functions: %d | Loops: %d | Decisions: %d | CC: %d | Recursion: %s\n",
           complexity, function_count, loops, decisions, cyclomatic_complexity,
           recursion_detected ? "Yes" : "No");

    return 0;
}
