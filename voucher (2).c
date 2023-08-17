#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct items {
    char item[20];
    float price;
    int qty;
};

struct orders {
    char customer[50];
    char date[50];
    int numOfItems;
    struct items itm[50];
};

void generateBillHeader(char name[50], char date[30]) {
    printf("\n \n");

    printf("\t Bill and Voucher System");
    printf("\n\t =======================");
    printf("\n");
    printf("\n Date        : %s",date);
    printf("\n Invoice for : %s\n", name);
    printf("\n");
    printf("============================================\n");
    printf("Items\t\t");
    printf("Quantity\t");
    printf("Total Price\t\t");
    printf("\n============================================");
    printf("\n\n");
}

void generateBillBody(char item[30], int qty, float price) {
    printf("%s\t\t",item);
    printf("%d\t\t",qty);
    printf("%.2f\t\t",qty * price);
    printf("\n");
}

void generateBillFooter(float total) {
    printf("\n");
    float disc = 0.08*total;
    float netTotal = total-disc;
    float VAT = 0.15*netTotal;
    float grandTotal=netTotal + VAT;
    printf("============================================\n");
    printf("\n Sub Total\t\t\t%.2f", total);
    printf("\n Discount @10 %s\t\t\t%.2f","%", disc);
    printf("\n\t\t\t\t============");

    printf("\n Net Total\t\t\t%.2f", netTotal);
    printf("\n VAT @9%s\t\t\t%.2f","%",VAT);
    printf("\n============================================");
    printf("\n Grand Total\t\t\t%.2f", grandTotal);
    printf("\n============================================\n");
}

int main() {
    float total;
    int opt, n;
    struct orders ord;
    struct orders order;
    char saveBill ='y', Flag = 'y';
    char name[50];

    FILE *fptr;

    while (Flag == 'y') {
        system("cls");
        float total = 0;
        int invoiceFound = 0;
        //dashboard
        printf("\n\t=============Billing and Voucher System===========");
        printf("\n\n Please select operation to perform: ");

        printf("\n\n (1). Generate Invoice");
        printf("\n\n (2). Display all Invoices");
        printf("\n\n (3). Search individual Invoice");
        printf("\n\n (4). Delete invoice by Name (Individual)");
        printf("\n\n (5). Delete the entire Invoice Recodes");
        printf("\n\n (6). Exits");

        printf("\n\n\t Enter Choice: ");
        scanf("%d", &opt);
        fgetc(stdin);

        switch(opt)
        {
            case 1:
                // ... Code for generating and saving invoices
                system("cls");
                printf("\n Enter customer Name: ");
                fgets(ord.customer,50,stdin);
                ord.customer[strlen(ord.customer)-1]=0; //captures the exact length of the string with no excess lines
                strcpy(ord.date,__DATE__);              //capturing the current date
                printf("\n Enter number of Items: ");
                scanf("%d",&n);
                ord.numOfItems = n;

                for(int i= 0; i<n; i++)
                {
                    fgetc(stdin);
                    printf("\n\n");
                    printf(" Enter the name of item %d \t : ", i+1);
                    fgets(ord.itm[i].item,20,stdin);
                    ord.itm[i].item[strlen(ord.itm[i].item)-1]=0;
                    printf(" Enter the quantity you need \t : ");
                    scanf("%d",&ord.itm[i].qty);
                    printf(" Enter the unit price \t         : ");
                    scanf("%f",&ord.itm[i].price);
                    total += ord.itm[i].qty * ord.itm[i].price;
                }
                generateBillHeader(ord.customer, ord.date);
                for(int i = 0; i < ord.numOfItems; i++)
                {
                    generateBillBody(ord.itm[i].item,ord.itm[i].qty,ord.itm[i].price);
                }
                generateBillFooter(total);

                printf("\n Do you want to save the invoice (y/n)? \t");
                scanf("%s", &saveBill);

                if (saveBill == 'y') {
                    fptr = fopen("lux.txt", "ab"); // Use "ab" for binary mode
                    fwrite(&ord, sizeof(struct orders), 1, fptr);

                    if(fwrite != 0)
                    printf("\n The invoice has been successfully saved!\n");
                    fclose(fptr);
                }
                else
                    printf("\n [ERROR] Information not saved !!\n");
                    fclose(fptr);

                break;

            case 2:
                //.. search invoice by user name
                system("cls");
                fptr = fopen("lux.txt", "rb"); // Use "rb" for binary mode
                printf("\n\t The invoices are as follows: \n");
                while (fread(&order, sizeof(struct orders), 1, fptr)) {
                    float tot = 0;
                    generateBillHeader(order.customer, order.date);
                    for (int i = 0; i < order.numOfItems; i++) {
                        generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                        tot += order.itm[i].qty * order.itm[i].price;
                    }
                    generateBillFooter(tot);
                    invoiceFound = 1;
                }
                fclose(fptr);
                if (!invoiceFound) {
                    printf("\n No invoices found!\n");
                }
                break;

            case 3:
                // ... Code for searching individual invoice
                printf("\n Enter Customer name \t: ");
                fgets(name, 50, stdin);
                name[strlen(name) - 1] = 0;
                system("cls");
                fptr = fopen("lux.txt", "rb"); // Use "rb" for binary mode
                printf("\n The invoices for %s: \n", name);
                invoiceFound = 0; // Reset the flag

                while (fread(&order, sizeof(struct orders), 1, fptr)) {
                    float tot = 0;
                    if (!strcmp(order.customer, name)) // Checks if the names match
                    {
                        generateBillHeader(order.customer, order.date);
                        for (int i = 0; i < order.numOfItems; i++) {
                            generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                            tot += order.itm[i].qty * order.itm[i].price;
                        }
                        generateBillFooter(tot);
                        invoiceFound = 1;
                    }
                }

                fclose(fptr);
                if (!invoiceFound) {
                    printf("\n Sorry, no invoices found for %s.\n", name);
                }
                break;
            case 4:
                //.. code to delete invoice by name
                printf("\n Enter Customer name to delete invoice: ");
                fgets(name, 50, stdin);
                name[strlen(name) - 1] = 0;

                FILE *tempFile = fopen("temp.txt", "wb"); // Open a temporary file for writing

                fptr = fopen("lux.txt", "rb"); // Open the original file for reading
                while (fread(&order, sizeof(struct orders), 1, fptr)) {
                    if (strcmp(order.customer, name) != 0) {
                        fwrite(&order, sizeof(struct orders), 1, tempFile); // Write all records except the one to be deleted
                    }
                }
                fclose(fptr);
                fclose(tempFile);

                // Remove the original file and rename the temporary file
                remove("lux.txt");
                rename("temp.txt", "lux.txt");

                printf("\n Invoice for %s has been deleted.\n", name);
                break;

            case 5:
                //... code to delete all the invoices
                system("cls");
                printf("\n Are you sure you want to delete all invoices (y/n)? ");
                char confirmation;
                scanf(" %c", &confirmation);

                if (confirmation == 'y' || confirmation == 'Y') {
                    // Open the file in write mode, effectively clearing its contents
                    fptr = fopen("lux.txt", "wb");
                    fclose(fptr);

                    printf("\n All invoices have been deleted.\n");
                } else {
                    printf("\n Deletion cancelled.\n");
                }
                break;


            case 6:
                printf("\n\t\t System shutting down, Bye :)\n");
                exit(0);
                break;

            default:
                printf("\n\t\t Invalid Choice!!\n");
                break;
        }

        // ... Rest of the main function code
        printf("\n Perform another operation (y/n): ");
        scanf("%s",&Flag);
    }

    printf("\n\t\t System shutting down, Bye :)\n");
    printf("\n\n");

    return 0;
}
