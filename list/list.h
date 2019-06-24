#ifndef _LIST_H_
#define _LIST_H_

#define STRUCT_OFFSET_OF(type, member) ((size_t) &((type *)0)->member)

/**
 * CONTAINER_OF - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:   the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */
#define CONTAINER_OF(ptr, type, member) ({          \
    const __typeof__( ((type *)0)->member ) *__mptr = (ptr); \
    (type *)( (char *)__mptr - STRUCT_OFFSET_OF(type,member) );})

struct list_head {
    struct list_head *next, *prev;
};

/* Initialise a list head to an empty list */
static inline void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
	list->prev = list;
}

/**
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(struct list_head *new,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(struct list_head *new, struct list_head *head)
{
  __list_add(new, head, head->next);
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
    __list_add(new, head->prev, head);
}

/** Take an element out of its current list, with or without
 * reinitialising the links.of the entry*/
static inline void list_del(struct list_head *entry)
{
	struct list_head *list_next = entry->next;
	struct list_head *list_prev = entry->prev;

	list_next->prev = list_prev;
	list_prev->next = list_next;

}

/**
 * list_entry - get the struct for this entry
 * @ptr:the &struct list_head pointer.
 * @type:the type of the struct this is embedded in.
 * @member:the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
    CONTAINER_OF(ptr, type, member)

#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
    pos = n, n = pos->next)

#endif // _LIST_H_
