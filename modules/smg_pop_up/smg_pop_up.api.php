<?php
/**
 * @file
 * Hooks provided by SMG Pop Up Module
 */

/**
 * If we're programmatically adding PURFs to a page (referred to as "Extra PURFs"), use
 * this hook to provide an array of information for that PURF.  This hook provides info
 * based on the passed in node
 */
function smg_purf_extra_purf($node){

}

/**
 * Similar to smg_purf_extra_purf, except this uses a "key" (same key that's used as the unique
 * id on the PURF link).  It returns an array where the key of the array is the unique id "key",
 * and the value is an array with information that can be used to substitute the smg_pop_up_inject_hidden_field_values
 * function.
 */
function smg_purf_extra_purf_by_key(){

}


?>