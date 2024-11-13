// HEADER CON LIBRERIAS QUE USO
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DEFINO LA ESTRUCTURA DE CONTACTO
typedef struct {
    int id;
    char name[30]; 
    int age; 
    char phone[50];
    char email[50];
    char location[50];
} Contact;

//GUARDADO DE CONTACTOS 
void saveContactToFile(Contact contact) {
    FILE *file = fopen("contacts.dat", "ab"); // "ab" abre el archivo en modo de adjuntar binario
    if (file == NULL) { // si el archivo no es encontrado imprime por pantalla "error"
        printf("Error opening file.\n");
        return;
    }

    fwrite(&contact, sizeof(Contact), 1, file); // escribe un contacto en el archivo
    fclose(file); // cierra el archivo
    printf("Contact saved successfully.\n"); // el contacto se guarda
}

// FUNCIONES QUE USO
int generateId(); // funcion para generar ID unico para cada contacto. Se usa int asi me devuelva un valor entero
void addContact(); // añadir contacto
void viewContacts(); // ver contactos
void searchContact(); // buscar contacto
void editContact(); // editar contacto
void deleteContact(); // eliminar contacto
void sortContacts(); // ordenar contactos
void deleteAllContacts(); //eliminar todos los contactos


// FUNCION PARA LEER EL ULTIMO ID GUARDADO
int readLastId() {
    FILE *file =fopen("lastId.dat", "rb");
    int lastId = 0; // si no se puede leer el archivo, comenzamos con el ID 0
    if (file != NULL) {
        fread(&lastId, sizeof(int), 1, file); // lee el ultimo ID guardado
        fclose(file);
    }
    return lastId;
}

// FUNCION PARA GUARDAR EL ULTIMO ID UTILIZADO
void saveLastId(int id) {
    FILE *file = fopen("lastId.dat", "wb");
    if(file != NULL) {
        fwrite(&id, sizeof(int), 1, file); //guarda el ultimo ID utilizado en el archivo
        fclose(file);
    } else {
        printf("Error saving the last ID.\n");
    }
}

// FUNCION GENERADORA ID
int generateId() {
    int lastId = readLastId(); // lee el ultimo ID guardado
    lastId++; // incrementa el numero de ID
    saveLastId(lastId); // guarda el nuevo ID
    return lastId;
} 

// FUNCION DE COMPARACION PARA 'qsort'
int compareContactsByName(const void *a, const void *b) { // se espera recibir dos punteros 
    const Contact *contactA = (const Contact *)a; // se convierte el puntero generico "a" en un puntero tipo Contact, que nos permite acceder a los datos del contacto
    const Contact *contactB = (const Contact *)b; // se convierte el puntero generico "b" en un puntero tipo Contact, similar a contactA
    return strcmp(contactA->name, contactB->name); // strcmp compara los "name" de contactA y contactB
    // strcmp devuelve valor negativo si contactA esta antes que contactB alfabeticamente
    // strcmp = 0 si los nombres son iguales
    // devuelve un valor positico si contactA deberia ir despues de contactB
    // el valor es utilizado por qsort para decidir el orden de los elementos del array
}

// MENU
void displayMenu() {
    printf("Contact Book Menu\n");
    printf("1. Add Contact\n");
    printf("2. View Contacts\n");
    printf("3. Search Contact\n");
    printf("4. Update Contact\n");
    printf("5. Delete Contact\n");
    printf("6. Sort Contacts\n");
    printf("7. Delete All Contacts\n");
    printf("8. Exit\n");
    printf("Enter your choice: \n");
}

// FUNCIONES MENU CON BUCLE DO..WHILE
int main() {
    int choice;
    
    do {
        displayMenu();
        scanf("%d", &choice);
        switch (choice) {
        case 1: addContact(); break;
        case 2: viewContacts(); break;
        case 3: searchContact(); break;
        case 4: editContact();   break;
        case 5: deleteContact(); break;
        case 6: sortContacts(); break;
        case 7: deleteAllContacts(); break;
        case 8: printf("Exiting Contact Book...\n"); break;
        default: printf("Invalid choice. Please, try again.\n");            
        }
    } while (choice != 8);

    return 0;
}

//FUNCIONALIDADES FUNCIONES VOID

//añadir contacto
void addContact() {
    Contact newContact; // se declara la variable llamada newContact de tipo Contact, donde se almacena temporalmente la información del nuevo contacto que se va a agregar
    newContact.id = generateId(); // asigna un ID unico

    printf("Enter name: ");
    scanf(" %[^\n]", newContact.name); // el formato %[^\n] permite capturar una línea completa con espacios hasta que se detecta un salto de línea (\n).
    printf("Enter age: ");
    scanf("%d", &newContact.age);
    printf("Enter phone: ");
    scanf(" %[^\n]", newContact.phone);
    printf("Enter email: ");
    scanf(" %[^\n]", newContact.email);
    printf("Enter location: ");
    scanf(" %[^\n]", newContact.location);

    // Aquí se utiliza para escribir los datos en un archivo
    FILE *file = fopen("contacts.dat", "ab"); // declaro un puntero file de tipo FILE y abre el archivo contacts.dat en modo binario. Esto significa que se abrirá el archivo para agregar datos al final, y si el archivo no existe, se creará.
    if (file != NULL) { // si el archivo se abrió correctamente, es decir file es distinto a NULL, significa que la operacion fue exitosa
        fwrite(&newContact, sizeof(Contact), 1, file); // escribe el contenido de la variable newContact en el archivo file. 
        fclose(file); // cierra el archivo para asegurar que los datos se guarden y liberar recursos del sistema
        printf("Contact added successfully.\n"); // imprime por pantalla que el contacto se agregó
    } else {
        printf("Error opening file.\n"); // si file es NULL, imprime un mensaje de error en la consola indicando que hubo un problema al abrir el archivo
    }
}
// los datos del nuevo contacto se guardan en el archivo contacts.dat

//ver contactos
void viewContacts() {
    FILE *file = fopen("contacts.dat", "rb"); // abre el archivo en modo de lectura binaria
    if (file == NULL) {
        printf("No contacts found.\n");
        return;
    }

    Contact contact;
    printf("\nList of Contacts:\n");
    while (fread(&contact, sizeof(Contact), 1, file)) {
        printf("ID: %d\n", contact.id);  // mostrar ID del contacto
        printf("Name: %s\n", contact.name);
        printf("Age: %d\n", contact.age);
        printf("Phone: %s\n", contact.phone);
        printf("Email: %s\n", contact.email);
        printf("Location: %s\n\n", contact.location);
    }

    fclose(file);
}

//buscar contacto
void searchContact() {
    char searchName[30]; // declaro un arreglo de caracteres para almacenar el nombre que se quiere buscar.
    int found = 0; // esta variable me indica si se encontro el contacto

    printf("Enter the name of the contact to search: ");
    scanf(" %[^\n]", searchName); // aca se lee el nombre que el usuario quiere buscar, permitiendo espacios

    FILE *file = fopen("contacts.dat", "rb"); // abrimos el archivo en modo de lectura binaria
    if (file == NULL) { // si el archivo no existe o hay un error
        printf("No contacts found or error opening file.\n"); // se muestra un mensaje
        return; // se retorna de la funcion
    }

    Contact contact;
    while (fread(&contact, sizeof(Contact), 1, file)) { // se leen los contactos uno x uno desde el archivo
        if (strcmp(contact.name, searchName) == 0) { // se compara el nombre leído del archivo con el nombre buscado.
            // si se encuentra un contacto con el nombre ingresado, mostramos la información
            printf("\nContact found:\n");
            printf("Name: %s\n", contact.name);
            printf("Age: %d\n", contact.age);
            printf("Phone: %s\n", contact.phone);
            printf("Email: %s\n", contact.email);
            printf("Location: %s\n\n", contact.location);
            found = 1; // si se encuentra el contacto, se imprime su información y se establece "found = 1"
            break; // se detiene la búsqueda una vez que se encuentra un contacto
        }
    }

    if (!found) { // si no se encuentra el contacto despues de leer todo el archivo, se imprime un mensaje 
        printf("Contact with name '%s' not found.\n", searchName); // no se encontro
    }

    fclose(file); // cerramos el archivo
}

//editar contacto
void editContact() { // declaro la funcion
    char searchName[30]; // variable
    int found = 0; // indica si se encuentra el contacto

    printf("Enter the name of the contact to edit: "); // mensaje que pide ingresar nombre a buscar
    scanf(" %[^\n]", searchName);

    FILE *file = fopen("contacts.dat", "rb+"); // abre el archivo en modo de lectura y escritura binaria.
    if (file == NULL) { // si file es NULL, salta error y termina funcion
        printf("No contacts found or error opening file.\n");
        return;
    }

    Contact contact;
    while (fread(&contact, sizeof(Contact), 1, file)) { // bucle while con fread para leer cada contacto del archivo
        if (strcmp(contact.name, searchName) == 0) { // con strcmp comparamos el nombre leido desde el archivo y el nombre que el usuario ingreso para buscar
            // si las dos cadenas son identicas, la funcion strcmp devuelve 0 y es contacto encontrado
            printf("Editing contact: %s\n", contact.name);

            // se solicitan los nuevos datos del contacto para almacenar en la estructura contact
            printf("Enter new name: ");
            scanf(" %[^\n]", contact.name);
            printf("Enter new age: ");
            scanf("%d", &contact.age);
            printf("Enter new phone: ");
            scanf(" %[^\n]", contact.phone);
            printf("Enter new email: ");
            scanf(" %[^\n]", contact.email);
            printf("Enter new location: ");
            scanf(" %[^\n]", contact.location);

            // mueve el puntero del archivo hacia atrás (justo al comienzo) para sobrescribir el contacto actual
            fseek(file, -(long)sizeof(Contact), SEEK_CUR); //utilizo -(long) porque -sizeof me daba error
            fwrite(&contact, sizeof(Contact), 1, file); // sobreescribe el contacto actual en el archivo con los datos nuevos que se han editado
            printf("Contact updated successfully.\n");
            found = 1; // indica si se ha encontrado el contacto
            break; // detiene el bucle después de editar el contacto
        }
    }

    if (!found) { // si el contacto no es encontrado se imprime mensaje de error
        printf("Contact with name '%s' not found.\n", searchName);
    }

    fclose(file); // cierra el archivo
}

//borrar contacto
void deleteContact() {
    int searchId;
    char searchName[30];
    int found = 0;

    printf("Enter the Id of the contact to delete: ");
    scanf(" %[^\n]", searchId);

    // abrir el archivo original (contacts.dat) en modo de lectura binaria
    FILE *file = fopen("contacts.dat", "rb");
    if (file == NULL) { // si file es igual a NULL (no se encuentra el archivo) imprime mensaje de error
        printf("No contacts found.\n");
        return;
    }

    // creo un archivo temporal para almacenar los contactos que no se van a borrar
    FILE *tempFile = fopen("temp.dat", "wb"); // modo de escritura binaria
    if (tempFile == NULL) { // si no se puede crear se imprime mensade de error
        printf("Error creating temporary file.\n");
        fclose(file); // se cierra el archivo
        return;
    }

    Contact contact;

    // se lee el archivo contacts.dat, si el nombre del contacto leido no coincide con el buscado, se escribe en el archivo temporal (temp.dat)
    while (fread(&contact, sizeof(Contact), 1, file)) {
        if (strcmp(contact.name, searchName) != 0) { // se compara el nombre del contacto en el archivo con el nombre a eliminar
            fwrite(&contact, sizeof(Contact), 1, tempFile); // si la comparacion es distinta a 0 (es decir no coinciden los 2 nombres) se escribe el contacto en el archivo temporal
        } else { //pero
            found = 1; // si los nombres coinciden, found sera igual a 1, y esto marca que se encontró el contacto a eliminar
        }
    }

    fclose(file); // se cierra el archivo (contacts.dat)
    fclose(tempFile); // se ciera el archivo temporal (temp.dat)

    // si se encontró el contacto, reemplazar el archivo original con el temporal
    if (found) {
        remove("contacts.dat");
        rename("temp.dat", "contacts.dat");
        printf("Contact deleted successfully.\n");
    } else {
        remove("temp.dat"); // eliminar el archivo temporal si no se encontró el contacto
        printf("Contact not found.\n");
    }
}

//ordenar contactos
void sortContacts() {
    FILE *file = fopen("contacts.dat", "rb"); // se abre el archivo en lectura binaria
    if (file == NULL) { // si file es NULL (no se puede abrir), imprime mensaje de error
        printf("No contacts found.\n");
        return;
    }

    // determinar el número de contactos en el archivo
    fseek(file, 0, SEEK_END); // se mueve el puntero del archivo al final
    long fileSize = ftell(file); // con ftell se determina el tamaño del archivo, y devuelve la posicion actual del archivo
    int numContacts = fileSize / sizeof(Contact); // se calcula el numero total de contactos, dividiendo el tamaño del archivo por el tamaño de un contacto
    rewind(file); // restablece el puntero del archivo al inicio, para poder leerlo desde el principio

    if (numContacts == 0) { // si el archivo no tiene contactos imprime un mensaje 
        printf("No contacts to sort.\n");
        fclose(file); // cierra el archivo
        return;
    }

    // leer todos los contactos en un array dinámico
    Contact *contactsArray = (Contact *)malloc(numContacts * sizeof(Contact)); // reserva memoria dinamica para un array que contendra todos los contactos
    if (contactsArray == NULL) { // si el array es NULL imprime un mensaje
        printf("Memory allocation failed.\n");
        fclose(file); // cierra el archivo
        return;
    }

    fread(contactsArray, sizeof(Contact), numContacts, file); // lee todos los contactos del archivo y los guarda en contactsArray
    fclose(file); // cierra el archivo

    // ordenar los contactos usando qsort, con la funcion previamente definida (ordenar alfabeticamente por name)
    qsort(contactsArray, numContacts, sizeof(Contact), compareContactsByName);


    // guardar los contactos ordenados en el archivo
    file = fopen("contacts.dat", "wb"); // abre el archivo en escritura binaria
    if (file == NULL) { // si file es NULL imprime mensaje
        printf("Error opening file for writing.\n");
        free(contactsArray); // libera la memoria dinamica que se uso para almacenar el array de contactos
        return;
    }

    fwrite(contactsArray, sizeof(Contact), numContacts, file); // funcion para escribir los contactos ordenados desde el array (contactsArray), de vuelta al archivo (contacts.dat)
    fclose(file); // cierra el archivo

    free(contactsArray); // libera la memoria dinamica
    printf("Contacts sorted successfully.\n");
}

//eliminar todos los contactos
void deleteAllContacts() {
    FILE *file = fopen("contacts.dat", "wb");

    if (file == NULL) {
        printf("Error opening file.\n");
        return; 
    }
    
    //no es necesario hacer nada mas, al abrirlo con "wb" el archivo se vacia automaticamente

    fclose(file);
    printf("All contacts have been deleted.\n");
}


       





