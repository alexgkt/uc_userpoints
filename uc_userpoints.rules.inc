<?php
/**
 * @file
 * Provide better integration into the rules group
 */

/**
 * Implements hook_rules_action_info().
 */
function uc_userpoints_rules_action_info(){

  $order_arg = array(
    'type' => 'uc_order',
    'label' => t('Order'),
  );

  $actions['uc_userpoints_assign_points'] = array(
    'label' => t('Award points upon payment completion'),
    'group' => t('Userpoints'),
    'base' => 'uc_userpoints_assign_points',
    'parameter' => array(
      'order' => $order_arg,
    ),
  );

  return $actions;
}

/***
 * @param $order
 * @return void
 */
function uc_userpoints_assign_points($order){

  // gives the user points for thier purchase, but checks via sql if they paid with points.  if so they get nothing also dont give points for tax and shipping.

  global $user;

  //$multiplier 	= variable_get(USERPOINTS_UC_EARN, 0);
  //$min 			= variable_get(USERPOINTS_MIN_PURCHASE, '0.00');
  $total = uc_userpoints_get_order_total($order);

  // Payment completed
	// Award points to buyer, if appropriate
	if ($order->uid) {
		// User id from the transaction
		$points = $total;
		$params = array (
			'uid' => $order->uid,
			'points' => $points,
			'operation' => 'insert',
			'description' => 'User made a purchase, awarding points (Ubercart Order ID ' . $order->order_id . ')',
		);
		$st = userpoints_userpointsapi($params);
	}
}
