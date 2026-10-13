#include "event_log.h"
#include <stddef.h>
#include <stdio.h>

static event_node_t s_pool[EVENT_LOG_MAX_ENTRIES];

static event_node_t *s_head = NULL;

static uint32_t s_pool_index = 0U;

static uint32_t s_event_count = 0U;

void event_log_init(void)
{
    s_head = NULL;
    s_pool_index = 0U;
    s_event_count = 0U;
}
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void event_log_add(event_type_t type, uint32_t timestamp, int16_t value)
{
    event_node_t *p_new = NULL;
    if (s_pool_index < EVENT_LOG_MAX_ENTRIES)
    {
        p_new = &s_pool[s_pool_index];
        s_pool_index++;
        s_event_count++;
    }
    else
    {
        
        event_node_t *p_current = s_head;

        while (p_current->p_next->p_next != NULL)
        {
            p_current = p_current->p_next;
        }

        
        p_new = p_current->p_next;

        
        p_current->p_next = NULL;
    }

    
    p_new->type = type;
    p_new->timestamp = timestamp;
    p_new->value = value;
    p_new->p_next = NULL;

    
    if (s_head == NULL)
    {
        s_head = p_new;
    }
    else if (timestamp > s_head->timestamp)
    {
        
        p_new->p_next = s_head;
        s_head = p_new;
    }
    else
    {
        
        event_node_t *p_current = s_head;

        while ((p_current->p_next != NULL) &&
               (p_current->p_next->timestamp >= timestamp))
        {
            p_current = p_current->p_next;
        }

        p_new->p_next = p_current->p_next;
        p_current->p_next = p_new;
    }
}

uint32_t event_log_get_count(void)
{
    return s_event_count;
}

void event_log_print_all(void)
{
    const event_node_t *p_current = s_head;

    while (p_current != NULL)
    {
        printf("Type: %u | Timestamp: %lu | Value: %d\n",
               (unsigned int)p_current->type,
               (unsigned long)p_current->timestamp,
               (int)p_current->value);

        p_current = p_current->p_next;
    }
}

const event_node_t *event_log_get_at(uint32_t index)
{
    const event_node_t *p_current = s_head;

    if (index >= s_event_count)
    {
        return NULL;
    }

    while (index > 0U)
    {
        p_current = p_current->p_next;
        index--;
    }

    return p_current;
}