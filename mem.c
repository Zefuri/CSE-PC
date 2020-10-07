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

        struct fb* newBlock = firstAdr + sizeof(struct bb);
        newBlock->size = firstAdr->size - size;

        head->first = newBlock;

        res->size = size;
    } else {
        res = NULL;
    }

    return res;
}

//-------------------------------------------------------------
// mem_free
//-------------------------------------------------------------
void mem_free(void* zone) {
   /* A COMPLETER */ 
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

    while ((curBlock->size < size) && curBlock->next) {
        precBlock = curBlock;
        curBlock = head->next;
    }

    if(curBlock->size >= size) {
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
