<?php

function uc_userpoints_discount_admin_discount(){
  $build = array();
  $rows = array();

  $result = db_query("SELECT * FROM {uc_userpoints_discount}");
  foreach ($result as $discount) {
    $row = array();
    $row[] = check_plain($discount->rank);
    $row[] = check_plain($discount->points);
    $row[] = check_plain(_get_discount_type($discount->discount));
    $row[] = l(t('edit'), 'admin/config/people/userpoints/discount/' . $discount->id . '/edit');
    $row[] = l(t('delete'), 'admin/config/people/userpoints/discount/' . $discount->id . '/delete');
    $rows[] = $row;
  }

  if (count($rows)) {
    $header = array(t('Rank'), t('Points'), t('Discount'), array('data' => t('Operations'), 'colspan' => 2));
    $build['methods'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  }

  return $build;
}

function uc_userpoints_discount_admin_discount_form($form, &$form_state, $id = 0){
  
  if ($id != 0 && ($method = db_query("SELECT * FROM {uc_userpoints_discount} WHERE id = :id", array(':id' => $id))->fetchObject())) {
    $form['id'] = array('#type' => 'value', '#value' => $id);
  }
  else {
    $method = (object) array(
      'rank' => '',
      'points' => '',
      'discount' => '',
    );
  }
  
  $form = array();
  
  $form['rank'] = array(
    '#type' => 'textfield',
    '#title' => t('Rank'),
  	'#size' => 30,
    '#default_value' => $method->rank,
    '#required' => TRUE,
  );
  
  $form['points'] = array(
    '#type' => 'textfield',
    '#title' => t('Points'),
  	'#size' => 30, 
    '#default_value' => $method->points,
    '#required' => TRUE,
  );
  
  $form['discount'] = array(
    '#type' => 'textfield',
    '#title' => t('Discount'),
  	'#description' => t('Use 0.05 for % and 5.00 for fixed amount.'),
  	'#size' => 30,
    '#default_value' => $method->discount ,
    '#required' => TRUE,
  );
  
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  if ($id != 0) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#validate' => array(),
      '#submit' => array('uc_userpoints_discount_admin_discount_delete_form'),
    );
  }
  
  return $form;
}

function uc_userpoints_discount_admin_discount_form_submit($form, &$form_state) {
  if (isset($form_state['values']['id'])) {
    drupal_write_record('uc_userpoints_discount', $form_state['values'], 'id');
    drupal_set_message(t('Discount was updated.'));
  }
  else {
    drupal_write_record('uc_userpoints_discount', $form_state['values']);
    drupal_set_message(t('New discount created.'));
  }

  $form_state['redirect'] = 'admin/config/people/userpoints/discount';
}

function uc_userpoints_discount_admin_discount_delete_form($form, &$form_state, $id){
  $form['id'] = array('#type' => 'value', '#value' => $id);

  return confirm_form($form, t('Do you want to delete this discount?'),
    'admin/config/people/userpoints/discount',
    t('This will remove the discount. This action can not be undone.'),
    t('Delete'));
}

function uc_userpoints_discount_admin_discount_delete_form_submit($form, &$form_state) {
  $id = $form_state['values']['id'];

  db_delete('uc_poslaju_methods')
    ->condition('id', $id)
    ->execute();

  drupal_set_message(t('Discount deleted.'));
  $form_state['redirect'] = 'admin/config/people/userpoints/discount';
}
