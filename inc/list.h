/*
List of uint and wide string, written in C. 
	© BladeMight
*/
#include <stdlib.h>
#ifndef LIST_T
	#define LIST_T
	typedef struct list {
		unsigned int val;
		wchar_t* sval;
		int lenght;
		struct list* next;
	} list_t;
	list_t* InitList(unsigned int val, wchar_t* sval) {
		list_t* list = malloc(sizeof(list_t));
		if (val != -1) {
			list->val = val;
			list->sval = sval;
			list->lenght = 1;
			list->next = NULL;
			debug(L">> init: [%d], {%s}, len: %d\n", val, sval, list->lenght);
		} else { list->lenght = 0; }
		return list;
	}
	void print_list(list_t* head) {
		list_t* current = head;
		if (head->lenght == 0) {
			debug(L"Empty list.\n");
		} else {
			debug(L"list lenght: %d, items: ", head->lenght);
			while (current != NULL) {
				debug(L"[%d]&{%s}", current->val, current->sval);
				current = current->next;
				if (current != NULL)
					debug(L", ");
			}
			debug(L"\n");
		}
	}
	int add(list_t* head, unsigned int val, wchar_t* sval) {
		// int lenght = 1;
		// debug(L"%d", head->lenght);
		if (head->lenght == 0 || head == NULL) {
			*head = *InitList(val, sval);
			debug(L"++> add: [%d], {%s}, len: %d\n", val, sval, head->lenght);
			return val;
		}

		list_t* current = head;
		// if (&head->val == NULL) {
			// head->val = val;
			// return val;
		// }
		while (current->next != NULL) {
				// lenght++;
				current = current->next;
		}
		head->lenght++;
		debug(L"++> add: [%d], {%s}, len: %d\n", val, sval, head->lenght);
		current->next = malloc(sizeof(list_t));
		current->next->val = val;
		current->next->sval = sval;
		current->next->next = NULL;
		head = current;
		return val;
	}
	int rem(list_t* head) {
		int retval = 0;
		/* if there is only one item in the list, remove it */
		if (head->lenght == 0) {
			debug(L"==> len: %d\n", 0);
			return retval;
		}
		if (head == NULL) {
			return 0;
		}
		// debug(L"ALIVE  %d\n", head);
		if (head->next == NULL) {
			retval = head->val;
			wchar_t* tsval = head->sval;
			head = InitList(-1, L"");
			debug(L"--> rem: [%d], {%s}, len: %d\n", retval, tsval, 0);
			return retval;
		}

		/* get to the last node in the list */
		list_t* current = head;
		while (current->next->next != NULL) {
			current = current->next;
		}
		head->lenght--;
		debug(L"--> rem: [%d], {%s}, len: %d\n", current->next->val, current->next->sval, head->lenght);

		/* now current points to the last item of the list, so let's remove current->next */
		retval = current->next->val;
		free(current->next);
		current->next = NULL;
		return retval;
	}
	int pop(list_t** head) {
		int retval = -1;
		list_t* next_node = NULL;
		if ((*head) == NULL)
			return -1;
		next_node = (*head)->next;
		next_node->lenght = (*head)->lenght - 1;
		retval = (*head)->val;
		wchar_t* tsval = (*head)->sval;
		debug(L"<-- rem: [%d], {%s}, len: %d\n", (*head)->val, tsval, next_node->lenght);
		free(*head);
		*head = next_node;

		return retval;
	}
	void clear(list_t** head) {
		free(*head);
		(*head) = malloc(sizeof(list_t));
		(*head)->lenght = 0;
		debug(L"--<Cleared>--\n");
	}
	int rem_index(list_t** head, int index) {	
		int retval = 0;
		// if out of bounds.
		if (index > (*head)->lenght - 1 || index < 0) {
			debug(L"?!> out of bounds: index: %d not in bounds of list: 0-%d.\n", index, (*head)->lenght - 1);
			return retval;
		}
		if (index == 0) {
			retval = (*head)->next->val;
			pop(&(*head));
			return retval;
		}
		if ((*head)->next == NULL) {
			retval = (*head)->val;
			free(*head);
			(*head)->lenght = 0;
			debug(L"--> len: %d\n", 0);
			return retval;
		}
		list_t* current = *head;
		for (int i = 0; i < index - 1; i++) {
			current = current->next;
		}
		
		list_t* temp_node = current->next;
		retval = temp_node->val;
		wchar_t* tsval = temp_node->sval;
		
		(*head)->lenght--;
		debug(L"!--> rem: [%d], {%s}, len: %d\n", retval, tsval, (*head)->lenght);
		current->next = temp_node->next;
		free(temp_node);
		return retval;
	}
	int index_val(list_t* head, int index) {
		int i = 0;
		list_t* current = head;
		while (current != NULL) {
			if (i == index)
				return current->val;
			else {
				current = current->next;
				i++;
			}
		}
		return -1;
	}
	wchar_t* index_sval(list_t* head, int index) {
		int i = 0;
		list_t* current = head;
		while (current != NULL) {
			if (i == index)
				return current->sval;
			else {
				current = current->next;
				i++;
			}
		}
		return L"";
	}
	int val_index(list_t* head, unsigned int val) {
		int index = 0;
		list_t* current = head;
		while (current != NULL) {
			if (current->val == val)
				return index;
			else {
				current = current->next;
				index++;
			}
		}
		return -1;
	}
	int sval_index(list_t* head, wchar_t* sval) {
		int index = 0;
		list_t* current = head;
		while (current != NULL) {
			if (current->sval == sval)
				return index;
			else {
				current = current->next;
				index++;
			}
		}
		return -1;
	}
#endif
