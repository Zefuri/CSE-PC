#include "mem.h"
#include "mem_os.h"
#include "common.h"
#include <stdio.h>

struct fb {
    size_t size;
    struct fb* next;
};

struct bb {
    size_t size;
    struct bb* next;
};

struct header {
    struct fb* first;
};

//-------------------------------------------------------------
// mem_init
//-------------------------------------------------------------
void mem_init() {
    char* adr = get_memory_adr();
    struct header* head = (struct header*) adr;
    struct fb* firstBlock = (struct fb*) (adr + sizeof(struct header));
    head->first = firstBlock;
    firstBlock->size = get_memory_size() - sizeof(struct header) - sizeof(struct fb);
    firstBlock->next = NULL;
}

//-------------------------------------------------------------
// mem_alloc
//-------------------------------------------------------------
void* mem_alloc(size_t size) {
    struct header* head = (struct header*) get_memory_adr();
    struct fb* firstAdr = mem_first_fit(head->first, size);

    struct bb* res;

    if(firstAdr) {
        res = (struct bb*) firstAdr;
        res->size = size;

        if((((void *) firstAdr) + sizeof(struct bb) + size) < (((void *) get_memory_adr()) + get_memory_size())) {
            struct fb *newBlock = ((void *) firstAdr) + sizeof(struct bb) + size;
            newBlock->size = firstAdr->size - size;

            head->first = newBlock;
        } else {
            head->first = NULL;
        }
    } else {
        res = NULL;
    }

    return res;
}

//-------------------------------------------------------------
// mem_free
//-------------------------------------------------------------
void mem_free(void* zone) {
    struct header* head = (struct header*) get_memory_adr();
    struct bb* toFreeBlock = (struct bb*) zone;
    struct fb* curBlock;
    struct fb* nearestFreeBlock;
    int coeffMin;

    if(head->first) {
        curBlock = head->first;
        nearestFreeBlock = curBlock;
        coeffMin = ((void *) toFreeBlock) - ((void *) nearestFreeBlock);

        while (curBlock->next) {
            curBlock = curBlock->next;
            int curCoeff = ((void *) toFreeBlock) - ((void *) curBlock);

            if(curCoeff < coeffMin) {
                coeffMin = curCoeff;
                nearestFreeBlock = curBlock;
            }
        }

        if(coeffMin > 0) {
            curBlock->size = curBlock->size + sizeof(struct bb) + toFreeBlock->size;

            if(curBlock->next == (((void *) curBlock) + sizeof(struct fb) + curBlock->size)) {
                curBlock->size = curBlock->size + sizeof(struct fb) + curBlock->next->size;
                curBlock->next = curBlock->next->next;
            }
        } else {
            struct fb* newBlock = (struct fb*) toFreeBlock;
            newBlock->next = nearestFreeBlock;

            if(newBlock->next == (((void *) newBlock) + sizeof(struct fb) + newBlock->size)) {
                newBlock->size = newBlock->size + sizeof(struct fb) + newBlock->next->size;
                newBlock->next = newBlock->next->next;
            }
        }
    } else {
        head->first = (struct fb*) toFreeBlock;
        head->first->size = get_memory_size() - sizeof(struct header) - sizeof(struct fb);
        head->first->next = NULL;
    }
}

//-------------------------------------------------------------
// Itérateur(parcours) sur le contenu de l'allocateur
// mem_show
//-------------------------------------------------------------
void mem_show(void (*print)(void *, size_t, int free)) {
   /* A COMPLETER */ 
}

//-------------------------------------------------------------
// mem_fit
//-------------------------------------------------------------
void mem_fit(mem_fit_function_t* mff) {
   /* A COMPLETER */ 
}

//-------------------------------------------------------------
// Stratégies d'allocation 
//-------------------------------------------------------------
struct fb* mem_first_fit(struct fb* head, size_t size) {
    struct fb* res;
    struct fb* precBlock;
    struct fb* curBlock = head;

    while ((curBlock->size < (size + sizeof(struct bb))) && curBlock->next) {
        precBlock = curBlock;
        curBlock = head->next;
    }

    if(curBlock->size >= (size + sizeof(struct bb))) {
        res = curBlock;
        if(precBlock) {
            precBlock->next = res + size;
        }
    } else {
        res = NULL;
    }

    return res;
}
//-------------------------------------------------------------
struct fb* mem_best_fit(struct fb* head, size_t size) {
   /* A COMPLETER */ 
    return NULL;

}
//-------------------------------------------------------------
struct fb* mem_worst_fit(struct fb* head, size_t size) {
   /* A COMPLETER */ 
    return NULL;
}
