#include <stdbool.h>
#if !defined(TMESH_OBSERVER_H)
#define TMESH_OBSERVER_H

/**
 * @file tmesh_observer.h
 * @brief Observer pattern implementation for ESP32-S3
 *
 * This module implements the observer design pattern to allow components
 * to subscribe to and receive notifications when a subject's state changes.
 * It provides a flexible event handling mechanism for the Toyama Mesh system.
 */

/** @brief Forward declaration of the observer structure */
typedef struct tmesh_observer tmesh_observer_t;

/** @brief Forward declaration of the subject structure */
typedef struct tmesh_subject tmesh_subject_t;

/**
 * @brief Function pointer type for observer update callbacks
 *
 * This callback is invoked whenever a subject's value changes
 *
 * @param observer The observer object being notified
 * @param subject The subject that has changed
 * @param user_data User-provided context data
 */
typedef void (*observer_update_fn)(tmesh_observer_t *observer,
                                   tmesh_subject_t *subject, void *user_data);

/**
 * @brief Observer structure that listens for changes in a subject
 */
struct tmesh_observer {
  /** Callback function to be invoked when the subject changes */
  observer_update_fn update;

  /** User-provided context data passed to the callback */
  void *user_data;

  /** Pointer to the next observer in the linked list */
  struct tmesh_observer *next;
};

/**
 * @brief Subject structure that maintains a state and notifies observers of
 * changes
 */
struct tmesh_subject {
  /** Linked list of observers registered to this subject */
  tmesh_observer_t *observers;

  /** Current value of the subject */
  int value;

  /** Number of observers currently registered */
  int observer_count;
};

/**
 * @brief API functions for the observer pattern implementation
 */

/**
 * @brief Initialize a subject
 *
 * Sets up a subject with default values and an empty observer list.
 *
 * @param subject Pointer to the subject to initialize
 * @return true if initialization was successful, false otherwise
 */
bool tmesh_subject_init(tmesh_subject_t *subject);

/**
 * @brief Add an observer to a subject
 *
 * Creates a new observer and adds it to the subject's list of observers.
 * The observer will be notified whenever the subject's value changes.
 *
 * @param subject Pointer to the subject
 * @param update_function Callback function to invoke when the subject changes
 * @param user_data User-provided context data to pass to the callback
 * @return Pointer to the new observer, or NULL if an error occurred
 */
tmesh_observer_t *tmesh_subject_add_observer(tmesh_subject_t *subject,
                                             observer_update_fn update_function,
                                             void *user_data);

/**
 * @brief Remove an observer from a subject
 *
 * Removes the specified observer from the subject's list and frees its memory.
 *
 * @param subject Pointer to the subject
 * @param observer Pointer to the observer to remove
 * @return true if the observer was found and removed, false otherwise
 */
bool tmesh_subject_remove_observer(tmesh_subject_t *subject,
                                   tmesh_observer_t *observer);

/**
 * @brief Update a subject's value and notify all observers
 *
 * Sets the subject's value and calls the update function of all registered
 * observers.
 *
 * @param subject Pointer to the subject
 * @param value New value for the subject
 * @return true if the update was successful, false otherwise
 */
bool tmesh_subject_update_value(tmesh_subject_t *subject, int value);

/**
 * @brief Deinitialize a subject
 *
 * Frees all observers and resets the subject to its initial state.
 *
 * @param subject Pointer to the subject to deinitialize
 * @return true if deinitialization was successful, false otherwise
 */
bool tmesh_subject_deinit(tmesh_subject_t *subject);

int tmesh_subject_get_value(tmesh_subject_t *subject);

#endif /* TMESH_OBSERVER_H */
