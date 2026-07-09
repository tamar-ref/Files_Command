// #include <stdio.h>

// #include "../types.h"

// #define BUFFER_SIZE 1024

// void execute_command(ParsedCommand cmd, char *response_out)
// {
//     FILE *file;

//     // --- פעולת CREATE ---
//     if (strcmp(cmd.command, "CREATE") == 0)
//     {
//         // פתיחת קובץ במצב כתיבה ("w") מייצרת אותו אם הוא לא קיים.
//         file = fopen(cmd.filename, "w");
//         if (file == NULL)
//         {
//             strcpy(response_out, "Error: Could not create file.");
//             return;
//         }
//         fclose(file);
//         sprintf(response_out, "File '%s' created successfully.", cmd.filename);
//     }

//     // --- פעולת WRITE ---
//     else if (strcmp(cmd.command, "WRITE") == 0)
//     {
//         // מצב "w" דורס את מה שהיה בקובץ וכותב מחדש
//         file = fopen(cmd.filename, "w");
//         if (file == NULL)
//         {
//             strcpy(response_out, "Error: Could not open file for writing.");
//             return;
//         }
//         fprintf(file, "%s", cmd.content);
//         fclose(file);
//         sprintf(response_out, "Written to '%s' successfully.", cmd.filename);
//     }

//     // --- פעולת APPEND ---
//     else if (strcmp(cmd.command, "APPEND") == 0)
//     {
//         // מצב "a" (Append) מוסיף לסוף הקובץ מבלי למחוק את הקודם
//         file = fopen(cmd.filename, "a");
//         if (file == NULL)
//         {
//             strcpy(response_out, "Error: Could not open file for appending.");
//             return;
//         }
//         fprintf(file, "%s", cmd.content);
//         fclose(file);
//         sprintf(response_out, "Appended content to '%s' successfully.", cmd.filename);
//     }

//     // --- פעולת READ ---
//     else if (strcmp(cmd.command, "READ") == 0)
//     {
//         file = fopen(cmd.filename, "r");
//         if (file == NULL)
//         {
//             strcpy(response_out, "Error: File not found.");
//             return;
//         }
//         // קריאת תוכן הקובץ לתוך התשובה שמחזירים ללקוח
//         memset(response_out, 0, BUFFER_SIZE);
//         fread(response_out, 1, BUFFER_SIZE - 1, file);
//         fclose(file);
//     }

//     // --- פעולת DELETE ---
//     else if (strcmp(cmd.command, "DELETE") == 0)
//     {
//         // פונקציית המערכת remove מוחקת קובץ בלינוקס
//         if (remove(cmd.filename) == 0)
//         {
//             sprintf(response_out, "File '%s' deleted successfully.", cmd.filename);
//         }
//         else
//         {
//             strcpy(response_out, "Error: Could not delete file (maybe it doesn't exist).");
//         }
//     }
// }