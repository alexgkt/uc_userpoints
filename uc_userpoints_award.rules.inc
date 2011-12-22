<?php
/**
 * @file
 * Provide better integration into the rules group
 */

/**
 * Implements hook_rules_action_info().
 */
function uc_userpoints_award_rules_action_info(){

  $order_arg = array(
    'type' => 'uc_order',
    'label' => t('Order'),
  );

  $actions['uc_userpoints_award_points'] = array(
    'label' => t('Award points upon payment completion'),
    'group' => t('Order'),
    'base' => 'uc_userpoints_award_points',
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
function uc_userpoints_award_points($order){

  // gives the user points for thier purchase, but checks via sql if they paid with points.  if so they get nothing also dont give points for tax and shipping.

  global $user;
  $curUserId	= $order->uid; //$user->uid; // $user->uid means nothing when the IPN call comes in...

  //$multiplier 	= variable_get(USERPOINTS_UC_EARN, 0);
  //$min 			= variable_get(USERPOINTS_MIN_PURCHASE, '0.00');
  $result 		= db_query("SELECT SUM(qty * price) FROM {uc_order_products} p, {uc_orders} o WHERE p.order_id = o.order_id and o.order_id = :oid and o.uid = :uid", array(':oid' => $order->order_id, 'uid' => $curUserId));
  $total		= $result->fetchField();

  // Payment completed
	// Award points to buyer, if appropriate
	if ($curUserId) {
		// User id from the transaction

		if ($total <= 0) {
			$points = 0;
		}
		else {
			$points = $total;
			$params = array (
				'uid' => $curUserId,
				'points' => $points,
				'operation' => 'insert',
				'description' => 'User made a purchase, awarding points (Ubercart Order ID ' . $order->order_id . ')',
			);
			$st = userpoints_userpointsapi($params);
		}
	}
}
