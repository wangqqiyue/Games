#include "Global.h"

State g_transition_table[][_event_total] = {
	/*_left_press _right_press _space_press _space_release _up_press _down_press _none */
	{GoingLeft, GoingRight, GettingForce, Idle, TurningUp, TurningDown, Idle},/*Idle*/
	{GoingLeft, GoingRight, GettingForce, Idle, TurningUp, TurningDown, Idle},/*GoingLeft*/
	{GoingLeft, GoingRight, GettingForce, Idle, TurningUp, TurningDown, Idle},/*GoingRight*/
	{GoingLeft, GoingRight, GettingForce, Idle, TurningUp, TurningDown, Idle},/*TuringUp*/
	{GoingLeft, GoingRight, GettingForce, Idle, TurningUp, TurningDown, Idle},/*TuringDown*/
	{GettingForce, GettingForce, GettingForce, Shooting, GettingForce, GettingForce, GettingForce},/*GettingForce*/
	{Shooting, Shooting, Shooting, Shooting, Shooting, Shooting, Shooting},/*Shooting*/
	{Flying, Flying, Flying, Flying, Flying, Flying, Flying},/*Flying*/
	{Exploding, Exploding, Exploding, Exploding, Exploding, Exploding, Exploding},/*Exploding*/
};