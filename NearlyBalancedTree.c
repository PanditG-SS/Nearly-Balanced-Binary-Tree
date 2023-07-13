#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Node structure consists of :
typedef struct node
{
    int value;
    int size;
    struct node *left;
    struct node *right;
} node;

// Global declaration of the array.
node *arr[10000001];
int idx;
double timer[10000001];

// This function creates a new node
node *makeNode(int val, int s)
{
    node *n = (node *)malloc(sizeof(node));
    n->left = NULL;
    n->right = NULL;
    n->value = val;
    n->size = s;
    return n;
}

// This function is used to print the tree.
void printTree(node *root)
{
    if (root == NULL)
    {
        return;
    }
    else
    {
        printTree(root->left);
        printf("Value = %d\tSize = %d\n", root->value, root->size);
        printTree(root->right);
    }
}

// This function checks if the tree is nearly balanced at node t
int NearlyBalanced(node *t)
{
    int flag = 1;

    if (t == NULL)
        return flag;

    if (t->left != NULL && t->left->size > (3 / 4.0 * (t->size)))
        flag = 0;
    if (t->right != NULL && t->right->size > (3 / 4.0 * (t->size)))
        flag = 0;

    return flag;
}

// Now we create a function to intialize the min and max array of pointers whenever the tree is found to non nearly balanced.
void ARRAYIntializer(node *p)
{
    if (p == NULL)
        return;
    else
    {
        ARRAYIntializer(p->left);
        arr[idx] = p;
        idx = idx + 1;
        ARRAYIntializer(p->right);
    }
}

/*
Transforming a subtree, say T′, into a perfectly balanced BST,you should ideally store (the pointers to)
all the nodes of T ′in the increasing order of their key values .

Further more, we create the perfectly balanced tree via this function.
*/
node *ArrConverterBST(int start, int end)
{
    if (start > end)
    {
        return NULL;
    }
    else
    {
        int mid = (start + end) / 2;

        arr[mid]->size = end - start + 1;

        arr[mid]->left = ArrConverterBST(start, mid - 1);

        arr[mid]->right = ArrConverterBST(mid + 1, end);

        return arr[mid];
    }
}

// This function starts the entire process of Perfect Tree making.
node *perfectTree(node *nodep)
{
    idx = 0;
    ARRAYIntializer(nodep);

    node *PerfectTree = ArrConverterBST(0, nodep->size - 1);
    return PerfectTree;
}

// This function is used to insert a number into the tree.
node *insertNode(int val, node *root1)
{
    node *root = root1;
    node *r = root1;

    node *leafNode = makeNode(val, 1);

    if (val == 1)
        return leafNode;

    //First insert the node.
    while (root != NULL)
    {
        root->size++;
        if (val > root->value)
        {
            if (root->right == NULL)
            {
                root->right = leafNode;
                break;
            }
            else
            {
                root = root->right;
            }
        }else
        if (val < root->value)
        {
            if (root->left == NULL)
            {
                root->left = leafNode;
                break;
            }
            else
            {
                root = root->left;
            }
        }
        
    }
    //As soon as the node is inserted , Check for non nearly balanced from top.
    root = r;
    node *prevNode = NULL;
    while (root != NULL)
    {

        if (!NearlyBalanced(root))
        {
            root = perfectTree(root);

            if (prevNode != NULL)
            {
                if (prevNode->value < root->value)
                {
                    prevNode->right = root;
                }
                else
                {
                    prevNode->left = root;
                }
            }
            else
            {
                root1 = root;
            }
            
        }
        prevNode = root;
        root = root->right;
    }

    return root1;
}

int main()
{
    node *root = NULL;
    double tt=0;
    for (int i = 1; i <=10000000; i++)
    {

        clock_t start_t, end_t;
        double total_t;
        start_t = clock();

        root = insertNode(i, root);

        end_t = clock();
        total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        tt += total_t;
        timer[i] = tt;
        printf("Time till %d insertion is : %f seconds \n ", i, timer[i]);
    }
    // printTree(root);
    FILE *ptr, *ptrgraph;

    // MAKING DATA FILES FOR PART3
    ptr = fopen("DATA1.csv", "w+");
    ptrgraph = fopen("GRAPH1.csv", "w+");
    fprintf(ptr, "#n ,\t time\n");
    fprintf(ptrgraph, "#nlogn ,\t time\n");
    for (int i = 50000; i <= 10000000; i += 50000)
    {
        fprintf(ptr, "%d ,\t %f\n", i, timer[i]);
        fprintf(ptrgraph, "%lf ,\t %f\n", i * (log2(i)), timer[i]);
    }

    fclose(ptr);
    fclose(ptrgraph);

    // MAKING DATA FILES FOR PART4
    ptr = fopen("DATA2.csv", "w+");
    fprintf(ptr, "#n, \t time\n");
    for (int i = 100000; i <= 1000000; i++)
    {
        fprintf(ptr, "%d ,\t %f\n", i, timer[i] - timer[i - 1]);
    }
    fclose(ptr);
}