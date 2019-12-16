#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int id;
    char nombre[20];
    float sueldo;
} eEmpleado;


eEmpleado* newEmpleado();
eEmpleado* newEmpleadoParam( int id, char* nombre, float sueldo);
eEmpleado** agrandarLista(eEmpleado** vec, int tam);
int setIdEmpleado(eEmpleado* e, int id);
int setNombreEmpleado(eEmpleado* e, char* nombre);
int setSueldoEmpleado(eEmpleado* e, float sueldo);
int mostrarEmpleado( eEmpleado* e);
int mostrarEmpleados( eEmpleado** e, int tam);
int guardarEmpleadosBinario(eEmpleado** lista,int tam,char* path);/**< archivo,tamanio,nombre del archivo */
int guardaEmpleadosCSV(eEmpleado** lista,int tam, char* path );

int main()
{
    int tam = 0; /**< cantidad de elementos en el Array, y me dice el indice del lugar libre. */
    int tam2 = 0;
    int cant; /**< Contar las variables cargadas por Fscanf */
    char buffer[3][30];/**< Buffer matris de 3 filas y 30 columnas (id,nombre,sueldo)"se guarda como txt"*/
    eEmpleado* auxEmpleado = NULL; /**<  estatica*/
    eEmpleado** lista = (eEmpleado**) malloc(sizeof(eEmpleado*));/**< pide un lugar en la memoria dinamica */
    //int auxInt;
    //float auxFloat;
    //char auxCad[100];
    if( lista == NULL)
    {
        printf("No se pudo asignar memoria\n");
        system("pause");
        exit(EXIT_FAILURE);
    }

    FILE* f = fopen("empleados.csv", "r");
    if( f == NULL)
    {
        printf("No se pudo abrir el archivo.");
        system("pause");
        exit(1);
    }



    /**< Encabezado */
    fscanf(f, "%[^,],%[^,],%[^\n]\n", buffer[0], buffer[1], buffer[2] );/**< lee todo hasta una coma(,)y al final hasta el \ n .*/
    /**< [] rango, & ^ leer todo hasta la coma (,)y la segunda coma la salta en el fscanf*/
    while( !feof(f))
    {

        cant = fscanf(f, "%[^,],%[^,],%[^\n]\n", buffer[0], buffer[1], buffer[2] );/**< cant = cantidad de variables cargadas por fscanf */
        if( cant == 3)
        {

            auxEmpleado = newEmpleadoParam(atoi(buffer[0]), buffer[1], atof(buffer[2]));/**< paso a auxEmpleado =  atoi (es el parseint en C), los datos de los buffer(0,1,2)/ atof (parseInt de flotantes en c) */

            if( auxEmpleado != NULL)
            {
                *(lista + tam) = auxEmpleado;/**< guardo el puntero de auxEmpleado a lista */

                //mostrarEmpleado(auxEmpleado);
                //system("pause");
                tam++;

                if((lista = agrandarLista(lista, tam + 1 )) == NULL)
                {
                    printf("No se pudo guardar en la lista.\n");
                }

            }
        }
        else
        {
            break;
        }
    }


    mostrarEmpleados(lista, tam);
    if(guardarEmpleadosBinario(lista,tam,"empleados.bin"))
    {
        printf("\nEmpleados guardados con exito.\n\n");
    }
    else
    {
        printf("\nNo se pudieron serializar los Empleados.\n");
    }

    fclose(f);


    /**< Leo desde Binario */
    eEmpleado** lista2 = (eEmpleado**) malloc(sizeof(eEmpleado*));
    if( lista2 == NULL)
    {
        printf("No se pudo asignar memoria\n");
        system("pause");
        exit(EXIT_FAILURE);
    }

    f = fopen("empleados.bin", "rb");
    if( f == NULL)
    {
        printf("No se pudo abrir el archivo.");
        system("pause");
        exit(1);
    }

    while( !feof(f))
    {
        auxEmpleado = newEmpleado();
        if(auxEmpleado != NULL)
        {
            cant = fread(auxEmpleado,sizeof(eEmpleado),1,f);   /**< fread(Puntero a Destino,tamanio,cantidad,archivo ) */

            if( cant ==  1)
            {
                *(lista2 + tam2) = auxEmpleado;/**< guardo el puntero de auxEmpleado a lista */
                tam2++;

                if((lista2 = agrandarLista(lista2, tam2 + 1 )) == NULL)
                {
                    printf("No se pudo guardar en la lista.\n");
                }

            }
        }
    }


    mostrarEmpleados(lista, tam2);
    if(guardarEmpleadosBinario(lista,tam2,"empleados.bin"))
    {
        printf("\nEmpleados guardados con exito.\n");
    }
    else
    {
        printf("\nNo se pudieron serializar los Empleados.\n");
    }

    printf("Empleados de la lista Binaria\n");

    fclose(f);


    /**< Crear un archivo csv de Empleados */


    if(guardaEmpleadosCSV(lista2,tam2,"listaEmpleados.csv"))
    {
        printf("Se guardaron los empleados en CSV\n.");
    }
    else
    {
        printf("Problema de guardar los empleados en CSV.\n");
    }



    return 0;
}


/** \brief Nuevo Empleado Set campos Empty
 * \param
 * \return
 */
eEmpleado* newEmpleado()
{
    eEmpleado* nuevo = (eEmpleado*) malloc( sizeof(eEmpleado));
    if(nuevo != NULL)
    {
        nuevo->id = 0;
        strcpy(nuevo->nombre, " ");
        nuevo->sueldo = 0;
    }
    return nuevo;
}


/** \brief Nuevo Empleador Parametros
 * \param
 * \return
 */

eEmpleado* newEmpleadoParam( int id, char* nombre, float sueldo)
{
    eEmpleado* nuevo = newEmpleado();
    if( nuevo != NULL)
    {
        if(setIdEmpleado(nuevo, id) &&
                setNombreEmpleado(nuevo, nombre)&&
                setSueldoEmpleado(nuevo, sueldo))
        {
            // printf("Empleado creado correctamente\n");
        }
        else
        {
            nuevo = NULL;
        }
    }

    return nuevo;
}

/** \brief Setea el id del Empleado
 * \param
 * \return
 */
int setIdEmpleado(eEmpleado* e, int id)
{
    int todoOk = 0;
    if( e != NULL && id >= 10000 && id <= 20000)
    {
        e->id = id;
        todoOk = 1;
    }
    return todoOk;
}

/** \brief Setea el sueldo del Empleado
 * \param
 * \return
 */
int setSueldoEmpleado(eEmpleado* e, float sueldo)
{
    int todoOk = 0;
    if( e != NULL && sueldo > 0)
    {
        e->sueldo = sueldo;
        todoOk = 1;
    }
    return todoOk;
}

/** \brief Setea el Nombre del Empleado
 * \param
 * \return
 */
int setNombreEmpleado(eEmpleado* e, char* nombre)
{
    int todoOk = 0;
    if( e != NULL && nombre != NULL && strlen(nombre) < 20)
    {
        strcpy(e->nombre, nombre);
        todoOk = 1;
    }
    return todoOk;
}

/** \brief Obtiene el Id del Empleado
 * \param
 * \return
 */
int getIdEmpleado(eEmpleado* e, int* id)
{
    int todoOk = 0;

    if( e != NULL && id != NULL)
    {
        *id = e->id;
        todoOk = 1;
    }
    return todoOk;
}

/** \brief Obtiene el sueldo del Empleado
 * \param
 * \return
 */
int getSueldoEmpleado(eEmpleado* e, float* sueldo)
{
    int todoOk = 0;

    if( e != NULL && sueldo != NULL)
    {
        *sueldo = e->sueldo;
        todoOk = 1;
    }
    return todoOk;
}

/** \brief Obtiene el Nombre del Empleado
 * \param
 * \return
 */
int getNombreEmpleado(eEmpleado* e, char* nombre)
{
    int todoOk = 0;

    if( e != NULL && nombre != NULL)
    {
        strcpy(nombre, e->nombre);
        todoOk = 1;
    }
    return todoOk;
}


/** \brief Muestra un Empleado
 * \param
 * \return
 */
int mostrarEmpleado( eEmpleado* e)
{
    int todoOk = 0;

    if( e != NULL)
    {

        printf("%d %s %.2f\n", e->id, e->nombre, e->sueldo);
        todoOk = 1;
    }

    return todoOk;

}

/** \brief Muestra los Empleados
 * \param
 * \return
 */
int mostrarEmpleados( eEmpleado** e, int tam)
{
    int todoOk = 0;

    if( e != NULL && tam > 0)
    {
        printf("Id   Nombre   Sueldo\n\n");
        for( int i=0; i < tam; i++)
        {
            mostrarEmpleado(*(e + i));
        }
        todoOk = 1;
    }

    return todoOk;

}

/** \brief Agranda la Lista
 * \param
 * \return
 */
eEmpleado** agrandarLista(eEmpleado** vec, int tam)
{
    eEmpleado** aux = (eEmpleado**) realloc(vec, sizeof(eEmpleado*) * tam);
    if( aux != NULL)
    {
        vec = aux;
    }
    return vec;
}


/** \brief Guarda Empleados en Binario
 * \param
 * \return
 */
int guardarEmpleadosBinario(eEmpleado** lista,int tam,char* path)
{
    int todoOk = 0;
    int cant;
    FILE* f;

    if(lista != NULL && path != NULL && tam > 0)
    {
        f = fopen(path,"wb");
        if(f == NULL)
        {
            return todoOk;
        }
        for(int i = 0; i< tam; i++)
        {
            cant = fwrite(*(lista + i), sizeof(eEmpleado), 1,f);  /**< fwrite(punteroOrigen,tamanio,cantidad,archivo) */
            if(cant < 1)
            {
                return todoOk;
            }
        }
        fclose(f);
        todoOk = 1;
    }

    return todoOk;
}

/** \brief Guarda Empleados en CSV
 * \param
 * \return
 */

int guardaEmpleadosCSV(eEmpleado** lista,int tam, char* path )
{
    int todoOk = 0;
    int cant;
    FILE* f;
    eEmpleado* auxEmp;

    if(lista != NULL && path != NULL && tam > 0)
    {
        f = fopen(path,"w");
        if(f == NULL)
        {
            return todoOk;
        }

        fprintf(f,"id,nombre,sueldo\n");

        for(int i = 0; i< tam; i++)
        {
            auxEmp = *(lista + i);
            cant = fprintf(f,"%d,%s,%.2f\n",auxEmp->id,auxEmp->nombre,auxEmp->sueldo);
            if(cant < 1)
            {
                return todoOk;
            }
        }
        fclose(f);
        todoOk = 1;
    }

    return todoOk;
}


/**
//----------------------
int ll_len(LinkedList* this)
{
    int returnAux = -1;

    if(this != NULL)
    {
        returnAux = this->size;
    }
    return returnAux;
}


//----------------------
static Node* getNode(LinkedList* this, int nodeIndex)
{
    Node* actual = NULL;
    int cont = 0;

    if(this != NULL)
    {

        if(nodeIndex >= 0 && nodeIndex < ll_len(this))
        {

            actual = this->pFirstNode;

            while(actual->pNextNode != NULL && cont != nodeIndex)
            {

                actual = actual->pNextNode;
                cont++;

            }
        }
    }

    return actual;
}

//----------------------
static int addNode(LinkedList* this, int nodeIndex,void* pElement)
{
    int returnAux = -1;
    Node* nuevoNodo = NULL;
    Node* auxiliar = NULL;

    if(this != NULL)
    {

        if(nodeIndex >= 0 && nodeIndex <= ll_len(this))
        {
            nuevoNodo = (Node*) malloc( sizeof(Node));
            if(nuevoNodo != NULL)
            {
                nuevoNodo->pElement = pElement;
                nuevoNodo->pNextNode = NULL;
                if(nodeIndex == 0)
                {
                    nuevoNodo->pNextNode = this->pFirstNode;
                    this->pFirstNode = nuevoNodo;
                    this->size++;
                    returnAux = 0;
                }
                else
                {

                    auxiliar  =  getNode(this,nodeIndex-1);
                    if(auxiliar != NULL)
                    {
                        nuevoNodo->pNextNode = auxiliar->pNextNode;
                        auxiliar->pNextNode = nuevoNodo;
                        this->size++;
                        returnAux = 0;
                    }
                }
            }

        }

    }
    return returnAux;
}
//----------------------
int ll_add(LinkedList* this, void* pElement)
{
    int returnAux = -1;

    if(this != NULL)
    {

        if( addNode(this,ll_len(this),pElement) == 0)
        {
            returnAux = 0;
        }
    }
    return returnAux;
}

//--------
Employee* employee_newParametros(char* idStr,char* nombreStr,char* horasTrabajadasStr,char* sueldoStr)
{
    Employee* nuevo = employee_new();
    int id;
    int horasTrabajadas;
    int sueldo;

    id = atoi(idStr);
    horasTrabajadas = atoi(horasTrabajadasStr);
    sueldo = atoi(sueldoStr);

    if( nuevo != NULL)
    {
        if(employee_setId(nuevo, id) &&
                employee_setNombre(nuevo, nombreStr)&&
                employee_setHorasTrabajadas(nuevo,horasTrabajadas)&&
                employee_setSueldo(nuevo, sueldo))
        {
            // printf("Empleado creado correctamente\n");
        }
        else
        {
            nuevo = NULL;
        }
    }

    return nuevo;
}

//------------------
void* ll_get(LinkedList* this, int index)
{
    void* returnAux = NULL;
    Node* aux = NULL;

    if(this!= NULL && index >= 0 && index < ll_len(this))
    {

        aux = getNode(this,index);

        returnAux = aux->pElement;

    }

    return returnAux;
}

//----------------------
int controller_addEmployee(LinkedList* pArrayListEmployee)
{
    int todoOk = 0;
    char buffer[4][20];
    Employee* filtro = NULL;
    int flagFor = 0;

    if(pArrayListEmployee != NULL)
    {

        printf("--------------  Alta de Empleados  --------------\n\n");
        printf("Ingrese ID:");
        fflush(stdin);
        gets(buffer[0]);

        for(int i = 0 ; i < ll_len(pArrayListEmployee) ; i++)
        {
            filtro = (Employee*) ll_get(pArrayListEmployee,i);
            if(filtro->id == atoi(buffer[0]))
            {
                printf("La ID ingresada ya se encuentra en uso\n");
                flagFor = 1;
                break;
            }
        }
        if(flagFor == 1 )
        {
            while(filtro->id == atoi(buffer[0]))
            {
                printf("Reingrese un ID valido:");
                fflush(stdin);
                gets(buffer[0]);
            }
        }

        printf("Ingrese nombre:");
        fflush(stdin);
        gets(buffer[1]);

        printf("Ingrese horas trabajadas:");
        fflush(stdin);
        gets(buffer[2]);
        while(atoi(buffer[2])< 0)
        {
            printf("ERROR. las horas no pueden ser menores a 0.\nReingrese:");
            fflush(stdin);
            gets(buffer[2]);
        }
        printf("Ingrese Sueldo:");
        fflush(stdin);
        gets(buffer[3]);
        while(atoi(buffer[3])< 0)
        {
            printf("ERROR. el sueldo no puede ser menor a 0.\nReingrese:");
            fflush(stdin);
            gets(buffer[3]);
        }

        ll_add(pArrayListEmployee,employee_newParametros(buffer[0],buffer[1],buffer[2],buffer[3])); //Todo ok. Agreago al Nuevo Empleado

        todoOk = 1;

    }

    return todoOk;
}

*/


