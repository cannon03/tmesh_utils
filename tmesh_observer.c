#include "tmesh_observer.h"
#include "esp_log.h"
#include <stdlib.h>

static const char *TAG = "TMESH_OBSERVER";

bool tmesh_subject_init(tmesh_subject_t *subject) {
  if (subject == NULL) {
    ESP_LOGE(TAG, "Invalid subject");
    return false;
  }
  subject->observers = NULL;
  subject->observer_count = 0;
  subject->value = 0;
  return true;
}

bool tmesh_subject_update_value(tmesh_subject_t *subject, int value) {
  if (subject == NULL) {
    ESP_LOGE(TAG, "Invalid subject received in tmesh_subject_update_value");
    return false;
  }
  subject->value = value;
  tmesh_observer_t *head = subject->observers;
  while (head != NULL) {
    head->update(head, subject, head->user_data);
    head = head->next;
  }
  return true;
}

bool tmesh_subject_deinit(tmesh_subject_t *subject) {
  if (subject == NULL) {
    ESP_LOGE(TAG, "Invalid subject received in tmesh_subject_deinit");
    return false;
  }
  tmesh_observer_t *head = subject->observers;
  while (head != NULL) {
    tmesh_observer_t *next = head->next;
    free(head);
    head = next;
  }
  subject->observers = NULL;
  subject->observer_count = 0;
  subject->value = 0;
  return true;
}

int tmesh_subject_get_value(tmesh_subject_t *subject) {
  if (subject == NULL) {
    ESP_LOGE(TAG, "Invalid subject received in tmesh_subject_get_value");
    return -1;
  }
  return subject->value;
}

tmesh_observer_t *tmesh_subject_add_observer(tmesh_subject_t *subject,
                                             observer_update_fn update_function,
                                             void *user_data) {
  if (subject == NULL) {
    ESP_LOGE(TAG, "Invalid subject");
    return NULL;
  }
  if (update_function == NULL) {
    ESP_LOGE(TAG, "Invalid update function");
    return NULL;
  }
  tmesh_observer_t *new_observer =
      (tmesh_observer_t *)malloc(sizeof(tmesh_observer_t));
  if (new_observer == NULL) {
    ESP_LOGE(TAG, "Failed to allocate memory for observer");
    return NULL;
  }
  new_observer->update = update_function;
  new_observer->user_data = user_data;
  new_observer->next = NULL;
  if (subject->observers == NULL) {
    subject->observers = new_observer;

  } else {
    tmesh_observer_t *head = subject->observers;
    while (head->next != NULL) {
      head = head->next;
    }
    head->next = new_observer;
  }
  subject->observer_count++;
  return new_observer;
}

bool tmesh_subject_remove_observer(tmesh_subject_t *subject,
                                   tmesh_observer_t *observer) {
  if (subject == NULL) {
    ESP_LOGE(TAG, "Invalid subject received in tmesh_subject_remove_observer");
    return false;
  }
  if (observer == NULL) {
    ESP_LOGE(TAG, "Invalid observer received in tmesh_subject_remove_observer");
    return false;
  }

  tmesh_observer_t *head = subject->observers;

  if (head == NULL) {
    ESP_LOGE(TAG, "No observers found in subject");
    return false;
  }

  if (head == observer) {
    subject->observers = observer->next;
    free(observer);
    subject->observer_count--;
    return true;
  }

  while (head->next != NULL) {
    if (head->next == observer) {
      head->next = head->next->next;
      free(observer);
      subject->observer_count--;
      return true;
    }
    head = head->next;
  }
  return false;
}
