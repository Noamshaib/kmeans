# include <stdio.h>
# include <stdlib.h>
# include <math.h>


struct cord
{
    double value;
    struct cord *next;
};
struct vector
{
    struct vector *next;
    struct cord *cords;
};


void print_list(struct vector* head);
int main(int argc, char *argv[]);
int calculateDim(struct vector *head);
/*int calculateNumOfPoints(struct vector *head);*/
int findClosestVec(struct vector *curr_vec, struct vector *centroids);
double calcDis(struct vector *vec1, struct vector *vec2);
struct vector *vecSum(struct vector *vec1, struct vector *vec2);
void buildCentroid(int numOfPoints, struct vector *pointSum, struct vector *head_new_cent);
int findMaxDis(struct vector *curr_cen, struct vector *p_curr_cen);
void freeVectors(struct vector *curr);


void print_list(struct vector* head){
    struct cord *cordinate;
    if ( head != NULL ){
        cordinate = head->cords;
        while (cordinate != NULL)
        {
            printf("%.4f",cordinate->value);
            if(cordinate->next != NULL){
                printf(",");
            }
            cordinate = cordinate->next;
        }
    printf("\n");
    print_list(head->next);
    }
}


int calculateDim(struct vector *head){
    int dim = 0;
    struct cord *cordinate;
    if(head == NULL){
        return 0;
    }
    cordinate = head->cords;
    while (cordinate != NULL)
        {
            dim++;
            cordinate = cordinate->next;
        }
    return dim;
}

 /* returns the index of the centroid with minimum distance from curr_vec */
int findClosestVec(struct vector *curr_vec, struct vector *centroids){
    struct vector *centroid;
    double temp_dis;
    double dis;
    int return_intex = 0;
    int index = 0;
    centroid = centroids;
    dis = calcDis(curr_vec, centroid);
    centroid = centroid->next;
    while (centroid != NULL){
        index++;
        temp_dis = calcDis(curr_vec, centroid);
        if(temp_dis < dis){
            dis = temp_dis;
            return_intex = index;
        }
        centroid = centroid->next;
    }
    return return_intex;
}

struct vector *initializePointSum(int dim){
    struct vector *head_vec;
    struct cord *head_cord, *curr_cord;
    int i;
    head_vec = malloc(sizeof(struct vector));
    head_cord = malloc(sizeof(struct cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;
    for(i=0; i<dim-1; i++){
        curr_cord->value = 0;
        curr_cord->next = malloc(sizeof(struct cord));
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
    }
    curr_cord->value = 0;
    curr_cord->next = NULL;
    head_vec->cords = head_cord;
    return head_vec;
}

/* Calculating distance between two vectors*/
double calcDis(struct vector *vec1, struct vector *vec2){
    double sum = 0;
    struct cord *head_cord_vec1, *head_cord_vec2;
    head_cord_vec1 = vec1->cords;
    head_cord_vec2 = vec2->cords;
    while (head_cord_vec1 != NULL){
        sum = sum + pow(((head_cord_vec2->value) - (head_cord_vec1->value)), 2);
        head_cord_vec2 = head_cord_vec2->next;
        head_cord_vec1 = head_cord_vec1->next;
    }
    return pow(sum, 0.5);
}


struct vector *vecSum(struct vector *vec1, struct vector *vec2){
    struct cord *head_cord_sum, *curr_cord_sum;
    struct vector *sum;
    struct cord *cord1, *cord2;
    sum = malloc(sizeof(struct vector));
    head_cord_sum = malloc(sizeof(struct cord));
    curr_cord_sum = head_cord_sum;
    curr_cord_sum->next = NULL;

    cord1 = vec1->cords;
    cord2 = vec2->cords;
    while (cord1->next != NULL){
        curr_cord_sum->value = (cord1->value) + (cord2->value);
        curr_cord_sum->next = malloc(sizeof(struct cord));
        curr_cord_sum = curr_cord_sum->next;
        curr_cord_sum->next = NULL;
        cord1 = cord1->next;
        cord2 = cord2->next;
    }
    curr_cord_sum->value = (cord1->value) + (cord2->value);
    curr_cord_sum->next = NULL;
    sum->cords = head_cord_sum;
    sum->next = NULL;
    return sum;
}

void buildCentroid(int numOfPoints, struct vector *pointSum, struct vector *head_new_cent){
    struct cord *curr_cord, *head_cord;
    struct cord *curr_sum, *head_sum;
    
    head_cord = malloc(sizeof(struct cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;
    head_sum = pointSum->cords;
    curr_sum = head_sum;

    while (curr_sum->next != NULL){
        curr_cord->value = (1.0/numOfPoints)*(curr_sum->value);
        curr_cord->next = malloc(sizeof(struct cord));
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
        curr_sum = curr_sum->next;
    }
    
    curr_cord->value = (1.0/numOfPoints)*(curr_sum->value);
    curr_cord->next = NULL;

    head_new_cent->cords = head_cord;
}


/* returns the maximum distance between two vectors */
int findMaxDis(struct vector *curr_cen, struct vector *p_curr_cen){
    struct vector *curr_vec, *curr_potential_vec;
    double temp_dis;
    double max_dis;
    curr_vec = curr_cen;
    curr_potential_vec = p_curr_cen;
    max_dis = calcDis(curr_vec, curr_potential_vec);
    curr_vec = curr_vec->next;
    curr_potential_vec = curr_potential_vec->next;

    while(curr_vec != NULL){
        temp_dis = calcDis(curr_vec, curr_potential_vec);
        if(temp_dis > max_dis){
            max_dis = temp_dis;
        }
        curr_vec = curr_vec->next;
        curr_potential_vec = curr_potential_vec->next;
    }
    return max_dis;
}

void freeVectors(struct vector *head){
    struct vector *curr_vec;
    struct vector *next_curr_vec;
    struct cord *curr_cord;
    struct cord *next_curr_cord;
    curr_vec = head;
    if(curr_vec == NULL){
        return;
    }
    while(curr_vec->next != NULL){
        /* free cords */
        curr_cord = curr_vec->cords;
        while(curr_cord->next != NULL){
            next_curr_cord = curr_cord->next;
            free(curr_cord);
            curr_cord = next_curr_cord;
        }
        free(curr_cord);
        next_curr_vec = curr_vec->next;
        free(curr_vec);
        curr_vec = next_curr_vec;
    }
    free(curr_vec);
}


int main(int argc, char *argv[]){

    struct vector *head_vec, *curr_vec,*head_centroid, *curr_centroid, *potential_head_centroid, *potential_curr_centroid, *pointSum, *next_pointSum;
    struct cord *head_cord, *curr_cord, *curr_sum, *curr_sum_next, *head_cent_cord, *curr_cent_cord;
    double n, eps;
    char c;
    int dim, iter, k, N, i, j, numOfPoints, m, max_dis, max_dis_temp; /*is_next_pointSum_init;*/
    int *partition;
    eps = 0.001;
    N = 0;
    numOfPoints = 0;
    m = 0;
    max_dis = 0;
    
    /*is_next_pointSum_init = 0;*/

    head_cord = malloc(sizeof(struct cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    curr_vec = head_vec;
    curr_vec->next = NULL;

    while (scanf("%lf%c", &n, &c) == 2)
    {

        if (c == '\n')
        {   
            N++;
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            curr_cord = head_cord;
            curr_cord->next = NULL;
            continue;
        }

        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(struct cord));
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
    }


    /* initializations of dim, k and iter */
    dim = calculateDim(head_vec);

    k = atoi(argv[1]);
    if(argc == 3){
        iter = atoi(argv[2]);
    }
    else{
        iter = 200;
    }
    
    /* check the requirements */

    if(iter<=1 || iter>=1000){
      printf("Invalid maximum iteration!");
      return 1; 
    }
    if(k<=1 || k>=N){
        printf("Invalid number of clusters!");
        return 1;
    }

    /* initialization of centroids */

    curr_vec = head_vec;
    curr_cord = curr_vec->cords;
    head_centroid = malloc(sizeof(struct vector));
    curr_centroid = head_centroid;
    for(i=0; i<k; i++){
        head_cent_cord = malloc(sizeof(struct cord));
        curr_cent_cord = head_cent_cord;

        while(curr_cord->next != NULL){
            curr_cent_cord->value = curr_cord->value;
            curr_cent_cord->next = malloc(sizeof(struct cord));
            curr_cent_cord = curr_cent_cord->next;
            curr_cent_cord->next = NULL;
            curr_cord = curr_cord->next;
        }
        curr_cent_cord->value = curr_cord->value;
        curr_cent_cord->next = NULL;
        
        curr_centroid->cords = head_cent_cord;

        if(i == k-1){
            curr_centroid->next = NULL;
        }

        else{
        curr_centroid->next = malloc(sizeof(struct vector));
        curr_centroid = curr_centroid->next;
        curr_centroid->next = NULL;
        curr_vec = curr_vec->next;
        curr_cord = curr_vec->cords;
        }
    }

    /* find closest centroid for every vector */
    curr_vec = head_vec;
    for(i=0; i<iter; i++){

        /* initialization of potential centroids */

        potential_head_centroid = malloc(sizeof(struct vector)); /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
        potential_curr_centroid = potential_head_centroid;

        partition = (int *)malloc(N * sizeof(int));
        j=0;
        while (curr_vec != NULL){
            partition[j] = findClosestVec(curr_vec, head_centroid);
            curr_vec = curr_vec->next;
            j++;
        }

         /* if its the last iteration we don't want to update the centroids */
        if(i == iter-1){
            print_list(head_centroid);
            free(partition);
            break;
        }

        curr_vec = head_vec;

        /* initialization of potential pointSum */
        pointSum = initializePointSum(dim);                     /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
        
        /* fill the potential centroids */

        for(j=0; j<k; j++){
            while (curr_vec->next != NULL){
                if(partition[m] == j){
                    next_pointSum = vecSum(pointSum, curr_vec);

                    /* free pointSum */
                    curr_sum = pointSum->cords;
                    while(curr_sum != NULL){
                        curr_sum_next = curr_sum->next; 
                        free(curr_sum);
                        curr_sum = curr_sum_next;
                    }
                    free(pointSum);
                    
                    pointSum = next_pointSum;
                    numOfPoints++; 
                }
                m++;
                curr_vec = curr_vec->next;
            }
            
            curr_vec = head_vec;
            buildCentroid(numOfPoints, pointSum, potential_curr_centroid); /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

            numOfPoints=0;
            m=0; 

            /* free pointSum */
            curr_sum = pointSum->cords;
            while(curr_sum != NULL){
                curr_sum_next = curr_sum->next; 
                free(curr_sum);
                curr_sum = curr_sum_next;
            }
            free(pointSum);
            pointSum = initializePointSum(dim);                     /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

            if (j < k-1){
                potential_curr_centroid->next = malloc(sizeof(struct vector));
                potential_curr_centroid = potential_curr_centroid->next;
                potential_curr_centroid->next = NULL;
            }
            else{                               /*last iteration*/
                potential_curr_centroid->next = NULL;
            }
        }

        potential_curr_centroid = potential_head_centroid;
        curr_centroid = head_centroid;

        /* check if all the distance between potential centroids and centroids are smaller than eps */
        while(curr_centroid != NULL){
            max_dis_temp = findMaxDis(curr_centroid, potential_curr_centroid);
            max_dis = (max_dis > max_dis_temp) ? max_dis : max_dis_temp;
            potential_curr_centroid = potential_curr_centroid->next;
            curr_centroid = curr_centroid->next;
        }

        if(max_dis < eps){
            print_list(head_centroid);
            free(partition);                  
            break;
        }

        /* free head_centroid */
        curr_sum = head_centroid->cords;
        while(curr_sum != NULL){
            curr_sum_next = curr_sum->next; 
            free(curr_sum);
            curr_sum = curr_sum_next;
        }
        free(head_centroid);

        head_centroid = potential_head_centroid;
        free(partition);
    }

    freeVectors(head_centroid);
    freeVectors(head_vec);

    return 0;
}