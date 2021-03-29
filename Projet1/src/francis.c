#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void descente(int *b, int *a, int taille, char op);
void final(int *b, int *a, int taille, char op);
void montee(int *tab, int taille, char op);
int operation(int, int, char);

int elem_neutre(char op)
{
    switch (op)
    {
    case '+':
        return 0;

    case '-':
        return 0;

    case '*':
        return 1;

    case '/':
        return 1;

    case 'm':
        return -1410065407;

    default:
        fprintf(stderr, "Error. Invalid opeator !\nPrograme ended with code error -1\n\a");
        exit(EXIT_FAILURE);
    }
}

// O(1) sans parallèle
int operation(int val1, int val2, char op)
{
    switch (op)
    {
    case '+':
        return val1 + val2;
    case '-':
        return val1 - val2;
    case '*':
        return val1 * val2;
    case '/':
        if (val2 != 0)
            return val1 / val2;
        else
        {
            fprintf(stderr, "Error. Divding by zero !\nPrograme ended with code error -1\n\a");
            exit(EXIT_FAILURE);
        }

    case 'm':
        if(val1>val2)return val1;
        else return val2;
    
    default:
        fprintf(stderr, "Error. Invalid opeator !\nPrograme ended with code error -1\n\a");
        exit(EXIT_FAILURE);
    }
}
// O(log(N)) en parallel et O(Nlog(N)) en normal
void descente(int *b, int *a, int taille, char op)
{
    int m = log2(taille);
    //if(boolean){
        b[0] = 0;

        for (int l = 1; l < m + 1; l++)
        {
            //printf("l = %d\n", l);
            int maxi = pow(2, l + 1);
            #pragma omp parallel for
            for (int j = pow(2, l) - 1; j < maxi - 1; j++)
            {
                //printf("\tj = %d\n",j);
                if ((j + 1) % 2 == 0)
                {
                    b[j] = b[(j - 1) / 2];
                }
                else
                {

                    b[j] = operation(b[(j - 1) / 2], a[j - 1], op);
                }
            }
        }
    /*}
    else{
        b[m+1] = 0;

        for (int l = m; l >= 1; l--)
        {
            //printf("l = %d\n", l);
            int maxi = pow(2, l + 1);
            #pragma omp parallel for
            for (int j = maxi; j >= pow(2, l) - 1; j--)
            {
                //printf("\tj = %d\n",j);
                if ((j + 1) % 2 == 0)
                {
                    b[j] = b[(j - 1) / 2];
                }
                else
                {

                    b[j] = operation(b[(j - 1) / 2], a[j - 1], op);
                }
            }
        }
    }*/
}


int isPowerOfTwo(int x)
{
    return (x != 0) && ((x & (x - 1)) == 0);
}

void montee(int *tab, int taille, char op)
{
    int m = log2(taille);
    //if(boolean){
        
        //printf("m = %d\n",m);
        for (int l = m - 1; l >= 0; l--)
        {
            //printf("l = %d:\n ",l);
            int max = pow(2, l + 1) - 1;
            #pragma omp parallel for
            for (int j = pow(2, l) - 1; j < max; j++)
            {
                //printf("\tj = %d\n",j);
                //printf("\ttab[%d], tab[%d] = %d, %d",2*j+1,2*j+2, tab[2*j+1], tab[2*j+2]);
                tab[j] = operation(tab[2 * j + 1], tab[2 * j + 2], op);
                //printf("\ttab[%d] = %d\n", j, tab[j]);
            }
            //printf("\n");
        }
    /*}
    else{
        //printf("m = %d\n",m);
        for (int l = 0; l <= m - 1; l++)
        {
            printf("l = %d:\n ",l);
            int max = pow(2, l + 1) - 1;
            #pragma omp parallel for
            for (int j = max-1; j >= pow(2, l) - 1; j--)
            {
                printf("\tj = %d\n",j);
                printf("\ttab[%d], tab[%d] = %d, %d",2*j+1,2*j+2, tab[2*j+1], tab[2*j+2]);
                tab[j] = operation(tab[2 * j + 1], tab[2 * j + 2], op);
                printf("\ttab[%d] = %d\n", j, tab[j]);
            }
            printf("\n");
        }
    }*/
}

int *create_copieTable(int *a, int taille, char op)
{

    int *tab_to_ret = (int *)malloc(sizeof(int *) * (pow(2, log2(taille) + 1) - 1));
    #pragma omp parallel for
    for (int i = 0; i < 2 * taille - 1; i++)
        tab_to_ret[i] = elem_neutre(op);
    #pragma omp parallel for
    for (int i = 0; i < taille; i++)
    {

        tab_to_ret[(taille - 1) + i] = a[i];
    }

    return tab_to_ret;
}

void final(int *b, int *a, int taille, char op)
{
    int m = log2(taille);
    int maxi = pow(2, m + 1) - 1;
    //if(boolean){

        #pragma omp parallel for
        for (int j = pow(2, m) - 1; j < maxi; j++)
        {
            //printf("a[%d] = %d, b[%d] = %d " ,j, a[j],j, b[j]);
            //printf("j = %d\t",j);
            b[j] = operation(b[j], a[j], op);
            //printf("b[%d] = %d\n", j, b[j]);
        }
        //printf("\n");
    /*}
    else{
        #pragma omp parallel for
        for (int j = maxi-1; j >= pow(2, m) - 1; j--)
        {
            //printf("j = %d\t",j);
            b[j] = operation(b[j], a[j], op);
            //printf("b[%d] = %d\n", j, b[j]);
        }
    }*/
}


void inverse_tab(int* t1, int* t, int n){
    for (int i = 0; i<n; i++)t1[n-1-i] = t[i];
}

void remplir(int *t1, int *t2, int *t, int n1, int n2)
{
    int index_on_t = 0;
    //#pragma omp parallel for
    for (int i = 0; i < n1; i++)
    {
        t1[i] = t[index_on_t++];
    }
    //#pragma omp parallel for
    for (int i = 0; i < n2; i++)
    {
        t2[i] = t[index_on_t++];
    }
}


void initialize(int*a_b_i, int * tabi,int ni, char op){
    #pragma omp parallel for
    for (int i = 0; i < 2 * ni - 1; i++)
        a_b_i[i] = elem_neutre(op);
    #pragma omp parallel for
    for (int i = 0; i < ni; i++)
    {
       a_b_i[(ni - 1) + i] = tabi[i];
    }
}

void copy(int *b, int *a, int taille)
{
    #pragma omp parallel for
    for (int i = 0; i < 2 * taille - 1; i++)
        b[i] = a[i];
}

int main(int argc, char **argv)
{

    FILE *fp;
    fp = fopen(argv[1], "r");
    if (!fopen)
    {
        fprintf(stderr, "coudln't open file name %s", argv[1]);
        exit(EXIT_FAILURE);
    }
    else
    {
        char ch;

        char buff[50];


        char op = '+';
        int n2;
       
        int i = 0, j = 0, nb_elements = 0;
        do
        {
            ch = fgetc(fp);
            buff[j++]=ch;
            if(ch==' ' || ch=='\t' || ch == '\n'){
                buff[j] = '\0';
                nb_elements++;
                j = 0;
            }
        }while(ch!=EOF);    
        int tab[nb_elements];
        fseek(fp, 0, SEEK_SET);
        j = 0;
        do
        {
            ch = fgetc(fp);
            buff[j++]=ch;
            if(ch==' ' || ch=='\t' || ch == '\n'){
                buff[j] = '\0';
                tab[i++] = atoi(buff);
                j = 0;
            }
        }while(ch!=EOF); 
        fclose(fp);
        int value_test_log2 = (int)log2(nb_elements);
        int reste = nb_elements - pow(2, value_test_log2);
        /**
         * Compute sum-prefix of Q and store them in array PSUM
        */
        //printf("reste = %d \n", reste);
        if (reste != 0)
        {
            if (isPowerOfTwo(reste) == 1)
            {
                n2 = pow(2, (int)log2(reste));
                //printf("n2 = %d\n", n2);
            }
            else
            {
                n2 = pow(2, (int)log2(reste) + 1);
                //printf("n2 = %d\n", n2);
            }
        }
        else
        {
            n2 = 0;
        }
        int n1 = pow(2, value_test_log2);
        

        /*printf("tab : \t");
        for(int i = 0;i<nb_elements; i++){
            printf("%d ", tab[i]);
        }
        printf("\n");*/
        int tab1[n1];
        int tab2[n2];

        remplir(tab1, tab2, tab, n1, n2);
        /*printf("tab1 : \t");
        for(int i = 0; i<n1; i++){
            printf("%d ", tab1[i]);
        }
        printf("\n");
        printf("tab2 : \t");
        for(int i = 0; i<n2; i++){
            printf("%d ", tab2[i]);
        }
        printf("\n");*/
        int size_a1 = pow(2, log2(n1) + 1) - 1;
        int *a1 = malloc(sizeof(int) * size_a1); //create_copieTable(tab1, n1, op);
        initialize(a1,tab1, n1, op);

        montee(a1, n1, op);
        int size_a2 = 0;
        if (reste != 0)
            size_a2 = pow(2, log2(n2) + 1) - 1;
        int *a2;
        if (reste != 0)
        {
            a2 = malloc(sizeof(int) * size_a2);
            initialize(a2, tab2, n2, op);
            montee(a2, n2, op);
        }
        /*printf("a1 :\t");
        for(int i = 0; i<2*n1-1; i++){
            printf("%d ", a1[i]);
        }
        printf("\n");
        printf("a2 :\t");
        for(int i = 0; i<2*n2-1; i++){
            printf("%d ", a2[i]);
        }
        printf("\n");*/
        int size_b1 = pow(2, log2(n1) + 1) - 1;
        int *b1 = malloc(sizeof(int) * size_b1); //create_copieTable(tab1, n1, op);
        initialize(b1,tab1,n1,op);
        int size_b2 = 0;
        if (reste != 0)
            size_b2 = pow(2, log2(n2) + 1) - 1;
        int *b2;
        if (reste != 0)
        {
           b2 = malloc(sizeof(int) * size_b2);
            //b2 = create_copieTable(tab2, n2, op);
            initialize(b2, tab2, n2,op);
            copy(b2, a2, n2);
        }
        copy(b1, a1, n1);
        /*printf("b1 : \t");
        for(int i = 0; i<2*n1-1; i++){
            printf("%d ", b1[i]);
        }
        printf("\n");
        
        printf("b2 : \t");
        for(int i = 0; i<2*n2-1; i++){
            printf("%d ",b2[i]);
        }
        printf("\n");*/

        descente(b1, a1, n1, op);
        /*printf("après descente : b1 :\t");
        for(int i = 0; i<2*n1-1; i++){
            printf("%d ", b1[i]);
        }*/
        final(b1, a1, n1, op);
        if (reste != 0)
        {
            descente(b2, a2, n2, op);
            /*printf("\naprès descente :b2 :\t");
            for(int i = 0; i<2*n2-1; i++){
                printf("%d ",b2[i]);
            }*/
            final(b2, a2, n2, op);
        }

        /*
        printf("\naprès finale \b1 : ");
        for(int i = 0; i<2*n1-1; i++){
            printf("%d ", b1[i]);
        }
        printf("\n");

        printf("\naprès finale \tb2 : ");
        for(int i = 0; i<2*n2-1; i++){
            printf("%d ",b2[i]);
        }
        printf("\n");
        
        printf("new processing ...\n\n");*/

        /*free(a1);
        free(b1);
        if (reste != 0)
        {
            free(b2);
            free(a2);
        }*/
        //we  store the current time in end

        //timeval is a struct with 2 parts for time, one in seconds and the other in
        //microseconds. So we convert everything to microseconds before computing
        //the elapsed time
        /*printf("%ld\n", ((end.tv_sec * 1000000 + end.tv_usec)
		  - (start.tv_sec * 1000000 + start.tv_usec)));*/
        int psum [nb_elements];
        int ssum[nb_elements];
        int smax[nb_elements];
        int pmax[nb_elements];
        int Ms[nb_elements];
        int Mp[nb_elements];
        i = 0;
        while(i<nb_elements){
            psum[nb_elements-1-i] = b1[size_b1-1-i];
            i++;
        }
        /**
         * Compute sum-suffix of Q and store them in array SSUM
        */
        int inverse_tab1[nb_elements];
        inverse_tab(inverse_tab1, tab, nb_elements);
        initialize(a1, inverse_tab1, n1, op);
        montee(a1, n1, op);
        initialize(b1, inverse_tab1, n1, op);
        copy(b1, a1, n1);
        descente(b1, a1, n1, op);
        final(b1, a1, n1, op);
        i = 0;
        while(i<nb_elements){
            ssum[i] = b1[size_b1-1-i];
            i++;
        }
        op = 'm';
        /**
         * Compute max-suffix of PSUM and store them in array SMAX*/    
        
        int inv_psum[nb_elements];
        inverse_tab(inv_psum, psum, nb_elements);
        initialize(a1, inv_psum, n1, op);
        montee(a1, n1, op);
        initialize(b1, inv_psum, n1, op);
        copy(b1, a1, n1);
        descente(b1, a1, n1, op);
        final(b1, a1, n1, op);
        i=0;
        while(i<nb_elements){
            smax[i] = b1[size_b1-1-i];
            i++;
        }
        /**
         * Compute max-prefix of SSUM and store them in array PMAX
        */

        initialize(a1, ssum, n1, op);
        montee(a1, n1, op);
        initialize(b1, ssum, n1, op);
        copy(b1, a1, n1);
        descente(b1, a1, n1, op);
        final(b1, a1, n1, op);
        i=0;
        while(i<nb_elements){
            pmax[nb_elements-1-i] = b1[size_b1-1-i];
            i++;
        }
        /*printf("psum : \t");
        for(int i =0;i<nb_elements;i++)printf("%d ", psum[i]);
        printf("\n");
        printf("ssum : \t");
        for(int i =0;i<nb_elements;i++)printf("%d ", ssum[i]);
        printf("\n");
        printf("smax : \t");
        for(int i =0;i<nb_elements;i++)printf("%d ", smax[i]);
        printf("\n");
        printf("pmax : \t");
        for(int i =0;i<nb_elements;i++)printf("%d ", pmax[i]);
        printf("\n");*/
        
        /**
         * for 1 <= i <= n do in parallel
        */
        int M[nb_elements];
        int maximum_val = elem_neutre('m');
        int index_of_max_val = -1,nb_element_sub_tab = 0;
        #pragma omp parallel for
        for(int i = 0;i<nb_elements;i++){
            Ms[i] = pmax[i] - ssum[i] + tab[i];
            Mp[i] = smax[i] - psum[i] + tab[i];
            int value = Ms[i] + Mp[i] - tab[i];
            if(value>maximum_val){
                index_of_max_val = i;
                maximum_val = value;
            }
            if(value == maximum_val){
                nb_element_sub_tab++; 
            }
            M[i] = value;
        }
        /*printf("M : \t");
        for(int i =0;i<nb_elements;i++)printf("%d ", M[i]);
        printf("\n");*/

        i = index_of_max_val;
        while (i<nb_element_sub_tab)
        {
            printf("%d ", tab[i]);
            i++;
        }
        




        return 0;
    }
}