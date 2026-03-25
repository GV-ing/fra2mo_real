#include <micro_ros_arduino.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <geometry_msgs/msg/twist.h>

// Variabili per micro-ROS
rcl_subscription_t subscriber;
geometry_msgs__msg__Twist msg;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

// Variabili per memorizzare la velocità richiesta
float linear_x = 0.0;
float angular_z = 0.0;

#define LED_PIN 25 // Il LED integrato sul Pico

// Macro per gestire eventuali errori di micro-ROS
#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

void error_loop(){
  while(1){
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
}

// ==========================================
// CALLBACK: Cosa fare quando arriva un messaggio /cmd_vel
// ==========================================
void cmd_vel_callback(const void * msgin) {
  const geometry_msgs__msg__Twist * twist_msg = (const geometry_msgs__msg__Twist *)msgin;
  
  linear_x = twist_msg->linear.x;
  angular_z = twist_msg->angular.z;

  // QUI INSERIREMO LA CINEMATICA INVERSA
  // Es: calcolare la velocità della ruota destra e sinistra
  // e mandare i segnali PWM al Driver Motori!
  
  // Per ora facciamo accendere il LED se il robot deve muoversi
  if (linear_x != 0.0 || angular_z != 0.0) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

void setup() {
  // Impostiamo il trasporto micro-ROS sulla porta Seriale USB nativa del Pico
  set_microros_transports();
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  delay(2000);

  allocator = rcl_get_default_allocator();

  // Inizializza il supporto micro-ROS
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // Crea il nodo chiamato "pico_motor_controller"
  RCCHECK(rclc_node_init_default(&node, "pico_motor_controller", "", &support));

  // Crea la Subcription al topic "/cmd_vel"
  RCCHECK(rclc_subscription_init_default(
    &subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
    "/cmd_vel"));

  // Crea l'esecutore per far girare la callback
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msg, &cmd_vel_callback, ON_NEW_DATA));
}

void loop() {
  // Lascia che micro-ROS processi i messaggi in arrivo (aspetta per max 100ms)
  delay(10);
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
  
  // (In futuro) Qui metteremo il codice per leggere gli encoder
  // e fare il loop di controllo PID per i motori!
}