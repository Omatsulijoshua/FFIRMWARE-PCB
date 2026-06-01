#pragma once

#include "esp_err.h"
#include "sdhs_types.h"

esp_err_t diagnostics_manager_init(void);
void diagnostics_manager_record_fault(sdhs_fault_code_t fault);
void diagnostics_manager_publish_status(void);

