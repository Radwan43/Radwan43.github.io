#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <stdlib.h>

char *replace_marker(char *haystack, const char *needle, const char *replacement) {
    char *result = NULL;
    int i, j, k;
    int needle_len = strlen(needle);
    int replacement_len = strlen(replacement);
    int haystack_len = strlen(haystack);
    int count = 0;

    for (i = 0; i < haystack_len; i++) {
        if (strstr(&haystack[i], needle) == &haystack[i]) {
            count++;
            i += needle_len - 1;
        }
    }

    result = (char *)malloc(haystack_len + (replacement_len - needle_len) * count + 1);
    if (!result) {
        perror("malloc failed");
        return NULL;
    }

    i = 0;
    j = 0;
    while (i < haystack_len) {
        if (strstr(&haystack[i], needle) == &haystack[i]) {
            for (k = 0; k < replacement_len; k++) {
                result[j++] = replacement[k];
            }
            i += needle_len;
        } else {
            result[j++] = haystack[i++];
        }
    }

    result[j] = '\0';
    return result;
}

int main() {
    const char *posts_directory = "./posts-txt";
    const char *blog_template_file = "./templates/blog.html";
    const char *index_template_file = "./templates/index.html";
    const char *public_posts_dir = "./posts";
    const char *output_index_file = "./index.html";

    char all_posts_html_for_index[10240] = "";

    DIR *d;
    struct dirent *dir;
    d = opendir(posts_directory);

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0 || strcmp(dir->d_name, ".DS_Store") == 0) {
                continue;
            }

            char *base_name = strdup(dir->d_name); // For filename manipulation
            char *txt_extension = strstr(base_name, ".txt");
            if (txt_extension != NULL) {
                *txt_extension = '\0';
            }

            char title[256] = "";
            char date[64] = "";
            char *separator = strstr(base_name, " || ");

            if (separator != NULL) {
                int title_len = separator - base_name;
                if (title_len < sizeof(title) - 1) {
                    strncpy(title, base_name, title_len);
                    title[title_len] = '\0';
                } else {
                    fprintf(stderr, "Title too long, truncating\n");
                    strncpy(title, base_name, sizeof(title) - 1);
                    title[sizeof(title) - 1] = '\0';
                }
                strcpy(date, separator + 4);
            } else {
                fprintf(stderr, "Separator not found in filename: %s\n", dir->d_name);
                strcpy(title, "Untitled");
                strcpy(date, "Unknown Date");
            }
            char post_content_filename[512];
            snprintf(post_content_filename, sizeof(post_content_filename), "%s/%s", posts_directory, dir->d_name);
            char *post_content = NULL;
            long post_content_length = 0;
            char line_buffer[2048];
            FILE *post_file_ptr = fopen(post_content_filename, "r");

            if (post_file_ptr) {
                char temp_content_buffer[10240] = "";
                bool paragraph_open = false;

                while (fgets(line_buffer, sizeof(line_buffer), post_file_ptr) != NULL) {
                    line_buffer[strcspn(line_buffer, "\n")] = 0;

                    bool is_blank_line = true;
                    for (int i = 0; line_buffer[i] != '\0'; i++) {
                        if (!isspace(line_buffer[i])) {
                            is_blank_line = false;
                            break;
                        }
                    }

                    if (is_blank_line) {
                        if (paragraph_open) {
                            strcat(temp_content_buffer, "</p>\n\n");
                            paragraph_open = false;
                        }
                    } else {
                        if (!paragraph_open) {
                            strcat(temp_content_buffer, "<p>");
                            paragraph_open = true;
                        }
                        strcat(temp_content_buffer, line_buffer);
                         strcat(temp_content_buffer, " ");
                    }
                }
                fclose(post_file_ptr);

                // Close the last paragraph if it's still open after reading all lines
                if (paragraph_open) {
                    strcat(temp_content_buffer, "</p>\n");
                }

                // Allocate memory and copy the formatted content
                post_content_length = strlen(temp_content_buffer);
                post_content = (char *)malloc(post_content_length + 1);
                strcpy(post_content, temp_content_buffer);


            } else {
                perror("Error opening blog post file");
                free(base_name);
                continue;
            }


            char *blog_template_content = NULL;
            long blog_template_length;
            FILE *blog_template_file_ptr = fopen(blog_template_file, "r");
            if (blog_template_file_ptr) {
                fseek(blog_template_file_ptr, 0, SEEK_END);
                blog_template_length = ftell(blog_template_file_ptr);
                fseek(blog_template_file_ptr, 0, SEEK_SET);
                blog_template_content = (char *)malloc(blog_template_length + 1);
                fread(blog_template_content, 1, blog_template_length, blog_template_file_ptr);
                blog_template_content[blog_template_length] = '\0';
                fclose(blog_template_file_ptr);
            }
            char *post_title_marker = "{{POST_TITLE}}";
            char *post_date_marker = "{{POST_DATE}}";
            char *post_content_marker = "{{POST_CONTENT}}";

            char *temp_html = replace_marker(blog_template_content, post_title_marker, title);
            free(blog_template_content);
            blog_template_content = temp_html;

            temp_html = replace_marker(blog_template_content, post_date_marker, date);
            free(blog_template_content);
            blog_template_content = temp_html;

            temp_html = replace_marker(blog_template_content, post_content_marker, post_content);
            free(blog_template_content);
            blog_template_content = temp_html;
            free(post_content);

            char output_post_filename[512];
            snprintf(output_post_filename, sizeof(output_post_filename), "%s/%s.html", public_posts_dir, title);

            FILE *output_post_file_ptr = fopen(output_post_filename, "w");
            if (output_post_file_ptr) {
                fprintf(output_post_file_ptr, "%s", blog_template_content);
                fclose(output_post_file_ptr);
                printf("Generated blog post: %s\n", output_post_filename);
            }
            free(blog_template_content);


            char article_link_html[2048]; // Adjust size as needed
            snprintf(article_link_html, sizeof(article_link_html),
                     "<div class=\"article-item\">\n"
                     "  <h3 class=\"article-title\"><a href=\"posts/%s.html\">%s</a></h3>\n"
                     "  <span class=\"article-date\">%s</span>\n"
                     "</div>\n",
                     title, title, date);

            strcat(all_posts_html_for_index, article_link_html);
            free(base_name);
        }
        closedir(d);
    }
    char *index_template_content = NULL;
    long index_template_length;
    FILE *index_template_file_ptr = fopen(index_template_file, "r");
    if (index_template_file_ptr) {
        fseek(index_template_file_ptr, 0, SEEK_END);
        index_template_length = ftell(index_template_file_ptr);
        fseek(index_template_file_ptr, 0, SEEK_SET);
        index_template_content = (char *)malloc(index_template_length + 1);
        fread(index_template_content, 1, index_template_length, index_template_file_ptr);
        index_template_content[index_template_length] = '\0';
        fclose(index_template_file_ptr);
    }

    char *blog_posts_marker = "{{BLOG_POSTS}}";
    char *final_index_html = replace_marker(index_template_content, blog_posts_marker, all_posts_html_for_index);
    free(index_template_content);


    FILE *output_index_file_ptr = fopen(output_index_file, "w");
    if (output_index_file_ptr) {
        fprintf(output_index_file_ptr, "%s", final_index_html);
        fclose(output_index_file_ptr);
        printf("Index HTML and blog posts generated successfully!\n");
    }
    free(final_index_html);

    return 0;
}


