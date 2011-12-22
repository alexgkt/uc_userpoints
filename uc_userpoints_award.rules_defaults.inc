<?php

/**
 * @file
 * Default rules configurations.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function uc_userpoints_default_rules_configuration() {
  $rule = rules_reaction_rule();
  $rule->label = t('Award points upon payment completion');
  $rule->active = TRUE;
  $rule->event('uc_payment_entered')
    ->action('uc_order_email');

  $configs['uc_userpoints_award_points'] = $rule;

  return $configs;
}
