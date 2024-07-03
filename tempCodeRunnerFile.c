
// void symbolTableGenerate()
// {
//     FILE *fp1;
//     fp1 = fopen("out3.txt", "r");
//     if (fp1 == NULL)
//     {
//         perror("Error opening file1");
//         return;
//     }

//     char str[1024];
//     char identName[1024];
//     char dataType[1024];
//     char scope[1024] = "global";

//     while (fscanf(fp1, "%s", str) != EOF)
//     {
//         if (isDataType(str))
//         {
//             // Handling variable declarations
//             if (strchr(str, '['))
//             {
//                 // Remove [ and ] characters from dataType
//                 memmove(dataType, str + 1, strlen(str) - 2);
//                 dataType[strlen(str) - 2] = '\0';

//                 // Read identifier name correctly
//                 fscanf(fp1, "%s", str);                       // Read the next token into `str`
//                 memmove(identName, str + 1, strlen(str) - 2); // Remove [ and ] characters
//                 identName[strlen(str) - 2] = '\0';            // Null-terminate the identifier name

//                 // Handling function declaration
//                 if (strchr(identName, '('))
//                 {
//                     // Extract function name
//                     char *parenPos = strchr(identName, '(');
//                     if (parenPos != NULL)
//                     {
//                         *parenPos = '\0'; // Null terminate at '(' to get function name
//                     }
//                     insert(identName, "func", dataType, scope, " ");
//                     strcpy(scope, identName);

//                     // Read parameters and add them to symbol table
//                     while (true)
//                     {
//                         fscanf(fp1, "%s", str);
//                         if (!isDataType(str))
//                             break;

//                         // Remove [ and ] characters from dataType
//                         memmove(dataType, str + 1, strlen(str) - 2);
//                         dataType[strlen(str) - 2] = '\0';

//                         // Read parameter name correctly
//                         fscanf(fp1, "%s", str);                       // Read the next token into `str`
//                         memmove(identName, str + 1, strlen(str) - 2); // Remove [ and ] characters
//                         identName[strlen(str) - 2] = '\0';            // Null-terminate the identifier name
//                         insert(identName, "var", dataType, scope, " ");
//                     }
//                 }
//                 else if (strchr(identName, '='))
//                 {
//                     // Variable initialization
//                     memmove(identName, identName + 1, strlen(identName) - 2);
//                     identName[strlen(identName) - 2] = '\0';
//                     fscanf(fp1, "%s", str);

//                     if (!(strchr(str, 'i') && strchr(str, 'd')))
//                     {
//                         insert(identName, "var", dataType, scope, str);
//                     }
//                 }
//                 else
//                 {
//                     // Variable declaration without initialization
//                     insert(identName, "var", dataType, scope, " ");
//                 }
//             }
//         }
//         // Handling identifier updates
//         else if (str[0] == '[' && str[1] == 'i' && str[2] == 'd')
//         {
//             // Handling identifier updates
//             fscanf(fp1, "%s", str);                       // Read the next token into `str`
//             memmove(identName, str + 1, strlen(str) - 2); // Remove [ and ] characters
//             identName[strlen(str) - 2] = '\0';            // Null-terminate the identifier name

//             fscanf(fp1, "%s", str);
//             if (strchr(str, '='))
//             {
//                 fscanf(fp1, "%s", str);
//                 if (!(strchr(str, 'i') && strchr(str, 'd')))
//                 {
//                     update(identName, scope, str);
//                 }
//             }
//         }
//         // Scope Management
//         else if (strchr(str, '}'))
//         {
//             // Handling end of scope
//             strcpy(scope, "global");
//         }
//     }

//     printf("\n\nSymbol Table:\n");
//     display();

//     fclose(fp1);
// }
