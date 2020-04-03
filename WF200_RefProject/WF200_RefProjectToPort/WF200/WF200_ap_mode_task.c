/*
 * WF200_ap_mode_task.c
 *
 *  Created on: 4 Mar 2020
 *      Author: SMurva
 */

/*******************************************************************************
 * INCLUDES
 ******************************************************************************/

#include "lwip/netifapi.h"
#include "lwip/opt.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "netif/ethernet.h"
#include "WF200_host_common.h"
#include "sl_wfx_host.h"
#include "event_groups.h"

/*******************************************************************************
 * DEFINITIONS
 ******************************************************************************/


/*******************************************************************************
 * VARIABLES
 ******************************************************************************/
ip_addr_t ap_ipaddr;
ip_addr_t ap_netmask;
ip_addr_t ap_gw;


/* SoftAP IP address */
uint8_t ap_ip_addr0      = AP_IP_ADDR0_DEFAULT;
uint8_t ap_ip_addr1      = AP_IP_ADDR1_DEFAULT;
uint8_t ap_ip_addr2      = AP_IP_ADDR2_DEFAULT;
uint8_t ap_ip_addr3      = AP_IP_ADDR3_DEFAULT;
uint8_t ap_netmask_addr0 = AP_NETMASK_ADDR0_DEFAULT;
uint8_t ap_netmask_addr1 = AP_NETMASK_ADDR1_DEFAULT;
uint8_t ap_netmask_addr2 = AP_NETMASK_ADDR2_DEFAULT;
uint8_t ap_netmask_addr3 = AP_NETMASK_ADDR3_DEFAULT;
uint8_t ap_gw_addr0      = AP_GW_ADDR0_DEFAULT;
uint8_t ap_gw_addr1      = AP_GW_ADDR1_DEFAULT;
uint8_t ap_gw_addr2      = AP_GW_ADDR2_DEFAULT;
uint8_t ap_gw_addr3      = AP_GW_ADDR3_DEFAULT;

char softap_ssid[32]                   = SOFTAP_SSID_DEFAULT;
char softap_passkey[64]                = SOFTAP_PASSKEY_DEFAULT;
sl_wfx_security_mode_t softap_security = SOFTAP_SECURITY_DEFAULT;
uint8_t softap_channel                 = SOFTAP_CHANNEL_DEFAULT;

extern uint8_t *pClient_MAC;

/*******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************/


void WF200_AP_ModeTask(void *arg);
sl_status_t WF200_HandleNetworkDetailsInput(void);



volatile bool scan_ongoing;
extern uint8_t scan_count_web;

extern scan_result_list_t scan_list[SL_WFX_MAX_SCAN_RESULTS];


// HTTP/1.1 200 OK,
// Server: FireAngel
// Content-Type: text/html; charset=utf-8
// Content-Length: 422
//
//
// <!DOCTYPE html>
// <html lang="en">
// <head><title>FireAngel</title></head>
// <body>
// <h1>FireAngel</h1>
// <h2>Scan Results</h2>
// <h3>Result &nbsp Ch &nbsp RSSI &nbsp&nbsp&nbsp MAC (BSSID) &nbsp&nbsp&nbsp Network (SSID)</h3>
// <ul>
// <li>01 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>02 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>03 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>04 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>05 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>06 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>07 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>08 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>09 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>10 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>11 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>12 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>13 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>14 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>15 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>16 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>17 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>18 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>19 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// <li>20 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
// </ul>
// <br>
// <br>
// <h2>Input Details</h2>
// <?phpif(isset($_GET["name"]))if(isset($_GET["pass"]))?>
// <form method="get" action="<?php echo $_SERVER["PHP_SELF"];?>">
// <label for="inputName">SSID:</label>
// <br>
// <input type="text" name="name" id="inputName">
// <br>
// <br>
// <label for="inputPass">PASS:</label>
// <br>
// <input type="text" name="pass" id="inputPass">
// <br>
// <br>
// <input type="submit" value="Submit">
// </form>
// </body>


char WebPageResponse[] = { /* HTTP/1.1 200 OK */
		                   'H',  'T',  'T',  'P',  '/',  '1',  '.',  '1', 0x20,  '2',  '0',  '0', 0x20,  'O',  'K', '\n',
						   'S',  'e',  'r',  'v',  'e',  'r',  ':', 0x20,  'F',  'i',  'r',  'e',  'A',  'n',  'g',  'e',     /* 16 */
						   'l', '\n',                                                                                         /* 2 */

						   /* Content-Type: text/html; charset=utf-8 */
						   'C',  'o',  'n',  't',  'e',  'n',  't',  '-',  'T',  'y',  'p',  'e',  ':', 0x20,  't',  'e',     /* 16 */
						   'x',  't',  '/',  'h',  't',  'm',  'l',  ';', 0x20,  'c',  'h',  'a',  'r',  's',  'e',  't',     /* 16 */
						   '=',  'u',  't',  'f',  '-',  '8', '\n',

						   /* Content-Length: 2967 */
						   'C',  'o',  'n',  't',  'e',  'n',  't', '-',  'L',  'e',  'n',  'g',  't',  'h',  ':', 0x20,      /* 16 */
						   '0',                                                                                               /* 1 */
						   '\n',                                                                                              /* 1 */
						   '\n',                                                                                              /* 1 */};

char WebPage[] = { /* HTTP/1.1 200 OK */
		           'H',  'T',  'T',  'P',  '/',  '1',  '.',  '1', 0x20,  '2',  '0',  '0', 0x20,  'O',  'K', '\n',     /* 16 */

				   /* Server: FireAngel */
				   'S',  'e',  'r',  'v',  'e',  'r',  ':', 0x20,  'F',  'i',  'r',  'e',  'A',  'n',  'g',  'e',     /* 16 */
				   'l', '\n',                                                                                         /* 2 */

				   /* Content-Type: text/html; charset=utf-8 */
				   'C',  'o',  'n',  't',  'e',  'n',  't',  '-',  'T',  'y',  'p',  'e',  ':', 0x20,  't',  'e',     /* 16 */
				   'x',  't',  '/',  'h',  't',  'm',  'l',  ';', 0x20,  'c',  'h',  'a',  'r',  's',  'e',  't',     /* 16 */
				   '=',  'u',  't',  'f',  '-',  '8', '\n',                                                           /* 7 */

				   /* Content-Length: 2967 */
				   'C',  'o',  'n',  't',  'e',  'n',  't', '-',  'L',  'e',  'n',  'g',  't',  'h',  ':', 0x20,      /* 16 */
				   '2',  '9',  '6', '7',                                                                              /* 4 */
				   '\n',                                                                                              /* 1 */
				   '\n',                                                                                              /* 1 */

				   /* <!DOCTYPE html> */
				   '<',  '!',  'D',  'O',  'C',  'T',  'Y',  'P',  'E', 0x20,  'h',  't',  'm',  'l',  '>',  '\n',    /* 16 */

				   /*  <html lang="en"> */
				   '<',  'h',  't',  'm',  'l', 0x20,  'l',  'a',  'n',  'g',  '=',  '"',  'e',  'n',  '"',  '>',     /* 16 */
				   '\n',                                                                                              /* 1 */

				   /* <head><title>FireAngel</title></head> */
				   '<',  'h',  'e', 'a',  'd',  '>',  '<',  't',  'i',  't',  'l',  'e',  '>',  'F',  'i',  'r',      /* 16 */
				   'e',  'A',  'n', 'g',  'e',  'l',  '<',  '/',  't',  'i',  't',  'l',  'e',  '>',  '<',  '/',      /* 16 */
				   'h',  'e',  'a', 'd',  '>', '\n',                                                                  /* 6 */

				   /* <body> */
				   '<',  'b',  'o',  'd',  'y',  '>', '\n',                                                           /* 7 */

				   /* <h1>FireAngel</h1> */
				   '<', 'h', '1', '>', 'F',  'i',  'r', 'e',  'A',  'n', 'g',  'e',  'l', '<', '/', 'h',              /* 16 */
				   '1', '>', '\n',                                                                                    /* 3 */

				   /* <h2>Scan Results</h2> */
				   '<', 'h', '2', '>', 'S', 'c', 'a', 'n', 0x20, 'R', 'e', 's', 'u', 'l', 't', 's',                   /* 16 */
				   '<', '/', 'h', '2', '>', '\n',                                                                     /* 6 */

				   // <h3>Result &nbsp Ch &nbsp RSSI &nbsp&nbsp&nbsp MAC (BSSID) &nbsp&nbsp&nbsp Network (SSID)</h3>
				   '<', 'h', '3', '>', 'R', 'e', 's', 'u', 'l', 't', 0x20, '&', 'n', 'b', 's', 'p',                   /* 16 */
				   0x20, 'C', 'h', 0x20, '&', 'n', 'b', 's', 'p', 0x20, 'R', 'S', 'S', 'I', 0x20, '&',                /* 16 */
				   'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 'M', 'A',                     /* 16 */
				   'C', 0x20, '(', 'B', 'S', 'S', 'I', 'D', ')', '&', 'n', 'b', 's', 'p', '&', 'n',                   /* 16 */
				   'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'e', 't', 'w', 'o', 'r', 'k',                    /* 16 */
				   0x20, '(', 'S', 'S', 'I', 'D', ')', '<', '/', 'h', '3', '>', '\n',                                 /* 13 */

				   // <ul>
				   '<', 'u', 'l', '>', '\n',                                                                          /* 5 */

				   // <li>01 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '0', '1', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>02 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '0', '2', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>03 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '0', '3', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>04 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '0', '4', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>05 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '0', '5', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>06 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '0', '6', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>07 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '0', '7', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>08 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '0', '8', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>09 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '0', '9', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>10 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '1', '0', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>11 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '1', '1', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>12 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '1', '2', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>13 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '1', '3', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>14 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '1', '4', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>15 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '1', '5', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>16 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '1', '6', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>17 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '1', '7', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>18 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '1', '8', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's' , 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>19 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '1', '9', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // <li>20 &nbsp 00 &nbsp&nbsp 0000 &nbsp&nbsp&nbsp 00:00:00:00:00:00 &nbsp&nbsp&nbsp No Result------------------</li>
				   '<', 'l', 'i', '>', '2', '0', 0x20, '&', 'n', 'b', 's', 'p', 0x20, '0', '0', 0x20, '&', 'n', 'b', 's',
				   'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b',
				   's', 'p','&', 'n', 'b', 's', 'p', 0x20, '0', '0', ':', '0', '0', ':', '0', '0', ':','0', '0', ':', '0', '0', ':',
				   '0', '0', 0x20, '&', 'n', 'b', 's', 'p', '&', 'n', 'b', 's', 'p','&', 'n', 'b', 's', 'p', 0x20, 'N', 'o', 0x20,
				   'R', 'e', 's', 'u', 'l', 't', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-',
				   '-', '-', '-', '-', '<', '/', 'l', 'i', '>', '\n',

				   // </ul>
				   '<', '/', 'u', 'l', '>', '\n',

				   // <br>
				   '<', 'b', 'r', '>', '\n',

				   // <br>
				   '<', 'b', 'r', '>', '\n',

				   // <h2>Input Details</h2>
				   '<', 'h', '2', '>', 'I', 'n', 'p', 'u', 't', 0x20, 'D', 'e', 't', 'a', 'i', 'l', 's', '<', '/', 'h', '2', '>', '\n',

				   // <?phpif(isset($_GET["name"]))if(isset($_GET["pass"]))?>
				   '<',  '?',  'p',  'h',  'p',  'i',  'f',  '(',
				   'i',  's',  's',  'e',  't',  '(',  '$',  '_',  'G',  'E',  'T',  '[',  '"',  'n',  'a',  'm',
				   'e',  '"',  ']',  ')',  ')',  'i',  'f',  '(',  'i',  's',  's',  'e',  't',  '(',  '$',  '_',
				   'G',  'E',  'T',  '[',  '"',  'p',  'a',  's',  's',  '"',  ']',  ')',  ')',  '?',  '>', '\n',

				   // <form method="get" action="<?php echo $_SERVER["PHP_SELF"];?>">
				   '<', 'f',  'o',  'r',  'm', 0x20,  'm',  'e',  't',  'h',  'o',  'd',  '=',  '"',  'g',  'e',  't',
				   '"', 0x20,  'a',  'c',  't',  'i',  'o',  'n',  '=',  '"',  '<',  '?',  'p',  'h',  'p', 0x20,
				   'e',  'c',  'h',  'o', 0x20,  '$',  '_',  'S',  'E',  'R',  'V',  'E',  'R',  '[',  '"',  'P',
				   'H',  'P',  '_',  'S',  'E',  'L',  'F',  '"',  ']',  ';',  '?',  '>',  '"',  '>', '\n',

				   // <label for="inputName">SSID:</label>
				   '<',  'l', 'a',  'b',  'e',  'l', 0x20,  'f',  'o',  'r',  '=',  '"',  'i',  'n',  'p',  'u',  't',  'N',
				   'a',  'm',  'e',  '"',  '>',  'S',  'S',  'I',  'D',  ':',  '<',  '/',  'l',  'a',  'b',  'e', 'l',  '>', '\n',

				   // <br>
				   '<',  'b',  'r',  '>', '\n',

				   // <input type="text" name="name" id="inputName">
				   '<',  'i',  'n',  'p',  'u',  't', 0x20,  't',  'y',  'p', 'e',  '=',  '"',  't',  'e',  'x',  't',  '"', 0x20,
				   'n',  'a',  'm',  'e',  '=',  '"',  'n', 'a',  'm',  'e',  '"', 0x20,  'i',  'd',  '=',  '"',  'i',  'n',  'p',
				   'u',  't',  'N',  'a', 'm',  'e',  '"',  '>', '\n',

				   // <br>
				   '<',  'b',  'r',  '>', '\n',

				   // <br>
				   '<',  'b',  'r',  '>', '\n',

				   // <label for="inputPass">PASS:</label>
				   '<',  'l',  'a',  'b',  'e',  'l', 0x20,  'f', 'o',  'r',  '=',  '"',  'i',  'n',  'p',  'u',  't',  'P',  'a',  's',
				   's',  '"',  '>',  'P', 'A',  'S',  'S',  ':',  '<',  '/',  'l',  'a',  'b',  'e',  'l',  '>', '\n',

				   // <br>
				   '<',  'b',  'r',  '>', '\n',

				   // <input type="text" name="pass" id="inputPass">
				   '<',  'i',  'n',  'p',  'u',  't', 0x20,  't',  'y',  'p',  'e',  '=',  '"',  't',  'e',  'x', 't',  '"', 0x20,
				   'n',  'a',  'm',  'e',  '=',  '"',  'p',  'a',  's',  's',  '"', 0x20,  'i', 'd',  '=',  '"',  'i',  'n',  'p',
				   'u',  't',  'P',  'a',  's',  's',  '"',  '>', '\n',

				   // <br>
				   '<',  'b',  'r',  '>', '\n',

				   // <br>
				   '<',  'b',  'r',  '>', '\n',

				   // <input type="submit" value="Submit">
				   '<',  'i',  'n',  'p',  'u',  't', 0x20,  't',  'y',  'p',  'e',  '=',  '"',  's',
				   'u',  'b',  'm',  'i',  't',  '"', 0x20,  'v',  'a',  'l',  'u',  'e',  '=',  '"',  'S',  'u',
				   'b',  'm',  'i',  't',  '"',  '>', '\n',

				   // </form>
				   '<',  '/',  'f',  'o',  'r',  'm',  '>', '\n',

				   // </body>
			       '<',  '/',  'b', 'o',  'd',  'y',  '>', '\n' };




/***************************************************************************//**
 * @brief
 *
 *
 * @param[in]
 *
 * @return
 *    None
 ******************************************************************************/
void WF200_AP_ModeTask(void *arg)
{
	sl_status_t WF200_DriverStatus = SL_STATUS_OK;
	const WF200_ControlInfo_t *pControlInfo = WF200_GetControlInfoPtr();

	if(pControlInfo != NULL)
	{
		// Log status
		PRINTF("WF200 AP Mode Task running\r\n");

		// Check the task is not already running, if it is something has gone wrong
	    if(pControlInfo->WF200_TaskInfo[WF200_AP_MODE_TASK].TaskRunning == false)
	    {
	    	WF200_SetTaskRunning(WF200_AP_MODE_TASK, true);
	    }
	    else
	    {
	    	// Log failure
	    	PRINTF("WF200_AP_ModeTask: ERROR - Task already running\n");
	    	WF200_DriverStatus = SL_STATUS_FAIL;
	    }

	    // Setup the NetIF stuff
	    WF200_DriverStatus = WF200_AP_ModeSetupNetIF();

        if(WF200_DriverStatus == SL_STATUS_OK)
        {
        	PRINTF("\r\n************************************************\r\n");
            PRINTF(" IP Address\r\n");
            PRINTF("************************************************\r\n");
            PRINTF(" IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&ap_ipaddr)[0], ((u8_t *)&ap_ipaddr)[1],
                   ((u8_t *)&ap_ipaddr)[2], ((u8_t *)&ap_ipaddr)[3]);
            PRINTF(" IPv4 Subnet mask : %u.%u.%u.%u\r\n", ((u8_t *)&ap_netmask)[0], ((u8_t *)&ap_netmask)[1],
                   ((u8_t *)&ap_netmask)[2], ((u8_t *)&ap_netmask)[3]);
            PRINTF(" IPv4 Gateway     : %u.%u.%u.%u\r\n", ((u8_t *)&ap_gw)[0], ((u8_t *)&ap_gw)[1],
                   ((u8_t *)&ap_gw)[2], ((u8_t *)&ap_gw)[3]);
            PRINTF("************************************************\r\n");

            // Initially start in AP mode
            WF200_DriverStatus = sl_wfx_start_ap_command(softap_channel,              // Channel
    		                                             (uint8_t*) softap_ssid,      // SSID
							     			             strlen(softap_ssid),         // SSID Length
												         0,                           // Hidden SSID
												         0,                           // Client Isolation
												         softap_security,             // Security Mode
												         0,                           // MFP
                                                         (uint8_t*) softap_passkey,   // Passkey
												         strlen(softap_passkey),      // Passkey Length
                                                         NULL,                        // Beacon IE Data
												         0,                           // Beacon IE Data Length
												         NULL,                        // Probe Response IE Data
												         0);                          // Probe Response IE Data Length


            if(WF200_DriverStatus != SL_STATUS_OK)
            {
            	// Log failure
            	PRINTF("WF200_AP_ModeTask: ERROR - Start AP Command Fail = %i\n", WF200_DriverStatus);
             	WF200_DEBUG_TRAP;
            }
        }

        if(WF200_DriverStatus == SL_STATUS_OK)
        {
        	WF200_DriverStatus = WF200_HandleNetworkDetailsInput();

            if(WF200_DriverStatus != SL_STATUS_OK)
            {
            	// Log failure
            	PRINTF("WF200_AP_ModeTask: ERROR - Handle Network details fail = %i\n", WF200_DriverStatus);
             	WF200_DEBUG_TRAP;
            }
        }

        // Now that the Network details are known
        // start the STA Mode
        if(WF200_DriverStatus == SL_STATUS_OK)
        {
        	WF200_DriverStatus = WF200_Setup_STA_Mode();

            if(WF200_DriverStatus != SL_STATUS_OK)
            {
            	PRINTF("WF200_AP_ModeTask: ERROR - Setup STA Mode fail = %i\n", WF200_DriverStatus);
             	WF200_DEBUG_TRAP;
            }
        }

        if(WF200_DriverStatus == SL_STATUS_OK)
        {
        	sl_wfx_disconnect_ap_client_command(pClient_MAC);
        	WF200_DriverStatus = sl_wfx_stop_ap_command();

            if(WF200_DriverStatus != SL_STATUS_OK)
            {
            	// Log failure
            	PRINTF("WF200_AP_ModeTask: ERROR - AP Stop Command Fail = %i\n", WF200_DriverStatus);
             	WF200_DEBUG_TRAP;
            }
        }

        if(WF200_DriverStatus == SL_STATUS_OK)
        {
        	WF200_DriverStatus = WF200_DeleteTask(WF200_AP_MODE_TASK);

        	if(WF200_DriverStatus != SL_STATUS_OK)
        	{
        		// Log failure
        		PRINTF("WF200_AP_ModeTask: ERROR - AP Delete Task fail = %i\n");
        	    WF200_DEBUG_TRAP;
        	}
        }
    }
    else
    {
    	// Log failure
    	PRINTF("WF200_AP_ModeTask: Error NULL Ptr\n");
    	WF200_DEBUG_TRAP;
    }
}


/***************************************************************************//**
 * @brief
 *
 *
 * @param[in]
 *
 * @return
 *    None
 ******************************************************************************/
sl_status_t WF200_AP_ModeSetupNetIF(void)
{
	err_t err;
	struct netif *pAP_netif = WF200_Get_AP_NetIF();
	sl_status_t WF200_DriverStatus = SL_STATUS_OK;

	/* Initialize the SoftAP information */
    IP_ADDR4(&ap_ipaddr,ap_ip_addr0,ap_ip_addr1,ap_ip_addr2,ap_ip_addr3);
    IP_ADDR4(&ap_netmask,ap_netmask_addr0,ap_netmask_addr1,ap_netmask_addr2,ap_netmask_addr3);
    IP_ADDR4(&ap_gw,ap_gw_addr0,ap_gw_addr1,ap_gw_addr2,ap_gw_addr3);

    /* Add SoftAP Interface */
    err = netifapi_netif_add(pAP_netif, /*ap_netif, */
                             &ap_ipaddr,
	                         &ap_netmask,
			                 &ap_gw,
			                 NULL,
	                         ap_ethernetif_init,
			                 tcpip_input);

    // Update status in the Control Structure
    WF200_Set_AP_NetIF_Status(WF200_AP_NETIF_ADDED);

    if(err != ERR_OK)
    {
    	// Log failure
	    PRINTF("WF200_AP_ModeSetupNetIF: ERROR - netif add fail = %i\n", err);
	    WF200_DriverStatus = SL_STATUS_FAIL;
	    WF200_DEBUG_TRAP;
    }

    if(WF200_DriverStatus == SL_STATUS_OK)
    {
	    err = netifapi_netif_set_default(pAP_netif);
    }

    if(err != ERR_OK)
    {
	    PRINTF("WF200_AP_ModeSetupNetIF: ERROR - netif set default failure = %i\n", err);
        WF200_DriverStatus = SL_STATUS_FAIL;
        WF200_DEBUG_TRAP;
    }

    return WF200_DriverStatus;
}


/***************************************************************************//**
 * @brief
 *
 *
 * @param[in]
 *
 * @return
 *    None
 ******************************************************************************/
sl_status_t WF200_Create_AP_ModeTask(void)
{
	BaseType_t status;
	TaskHandle_t taskHandle = NULL;
	sl_status_t WF200_DriverStatus = SL_STATUS_FAIL;
	const WF200_ControlInfo_t *pControlInfo = WF200_GetControlInfoPtr();

	if(pControlInfo == NULL)
	{
		// Log failure
		PRINTF("WF200_Create_AP_ModeTask: Error NULL Ptr\n");
		WF200_DEBUG_TRAP;
	}

	// Shouldn't be trying to create the Task if it is already created
	if(pControlInfo->WF200_TaskInfo[WF200_AP_MODE_TASK].TaskCreated == false)
	{
		status = xTaskCreate(WF200_AP_ModeTask,
			                 "WF200_AP_ModeTask",
						     WF200_AP_MODE_TASK_STACK_SIZE,
						     NULL,
						     WF200_AP_MODE_TASK_PRIORITY,
						     &taskHandle);

	    if (status == pdPASS)
	    {
	    	WF200_DriverStatus = SL_STATUS_OK;
		    WF200_SetTaskCreated(WF200_AP_MODE_TASK, true);
		    WF200_SetTaskHandle(WF200_AP_MODE_TASK, taskHandle);

		    // Log status
		    PRINTF("WF200_Create_AP_ModeTask: SUCCESS - Task Creation\n");
	    }
	    else
	    {
		    // Log failure
		    PRINTF("WF200_Create_AP_ModeTask: ERROR - Task Creation\n");
		    WF200_DEBUG_TRAP;
	    }
	}
	else
	{
		// Log failure
		PRINTF("WF200_Create_AP_ModeTask: ERROR - Trying to setup a task that is already created\n" );
		WF200_DEBUG_TRAP;
	}

	return WF200_DriverStatus;
}


/***************************************************************************//**
 * @brief
 *
 *
 * @param[in]
 *
 * @return
 *    None
 ******************************************************************************/
sl_status_t WF200_Setup_AP_Mode(void)
{
	sl_status_t WF200_DriverStatus = SL_STATUS_OK;
	const WF200_ControlInfo_t *pControlInfo = WF200_GetControlInfoPtr();

	if(pControlInfo == NULL)
	{
		// Log failure
		PRINTF("WF200_Setup_AP_Mode: Error NULL Ptr\n");
		WF200_DEBUG_TRAP;
	}

	// If not already running start the Comms and Event Task
	if((pControlInfo->WF200_TaskInfo[WF200_COMMS_TASK].TaskCreated == false) &&
	   (WF200_DriverStatus == SL_STATUS_OK))
	{
		// Start the comms task running
		WF200_DriverStatus = WF200_CreateCommsTask();

		if((WF200_DriverStatus == SL_STATUS_OK) &&
		   (pControlInfo->WF200_TaskInfo[WF200_COMMS_TASK].TaskCreated == false))
		{
			WF200_DriverStatus = WF200_CreateEventTask();
		}
	}

	// If WF200 is in Shutdown, wake it up
	if((pControlInfo->Awake == false) &&
	   (WF200_DriverStatus == SL_STATUS_OK))
	{
		// Wakeup the chip
		WF200_DriverStatus = WF200_Init();

		if(WF200_DriverStatus == SL_STATUS_OK)
    	{
			WF200_SetAwake(true);
    	}
	}

	// If STA mode is running shut it down
	if((pControlInfo->WF200_TaskInfo[WF200_STA_MODE_TASK].TaskCreated == true) &&
	   (WF200_DriverStatus == SL_STATUS_OK))
	{
		if(pControlInfo->STA_ModeNetworkConnected == true)
		{
			WF200_Set_STA_CommandedDisconnect(true);
			WF200_DriverStatus = sl_wfx_send_disconnect_command();
		}

		// Delete the task
		WF200_DriverStatus = WF200_DeleteTask(WF200_STA_MODE_TASK);
	}

	// If all good, setup the netif
	if(WF200_DriverStatus == SL_STATUS_OK)
	{
		WF200_DriverStatus = WF200_Create_AP_ModeTask();
	}

    return WF200_DriverStatus;
}


/***************************************************************************//**
 * @brief
 *
 *
 * @param[in]
 *
 * @return
 *    None
 ******************************************************************************/
sl_status_t WF200_HandleNetworkDetailsInput(void)
{
	err_t err;
	void *data;
	uint16_t len;
	char datax[500];
	uint32_t timeout = 0;
////    uint16_t idNotFound = 0;
	struct netconn *conn = NULL;
	struct netconn *newconn = NULL;
    struct netbuf *rxBuffer = NULL;
    sl_status_t WF200_DriverStatus = SL_STATUS_OK;

    // Create a connection structure
    conn = netconn_new(NETCONN_TCP);

    if(conn == NULL)
    {
    	PRINTF("ERROR: WF200_HandleNetworkDetailsInput - netconn_new error");
    	WF200_DriverStatus = SL_STATUS_FAIL;

    	WF200_DEBUG_TRAP;
    }

    // Bind the connection to port 2000
    // on any local IP address
    err = netconn_bind(conn, NULL, 2000);

    if (err != ERR_OK)
    {
    	PRINTF("ERROR: WF200_HandleNetworkDetailsInput - netconn_bind error");
    	WF200_DriverStatus = SL_STATUS_FAIL;

    	WF200_DEBUG_TRAP;
    }

    // Tell the connection to listen for
    // incoming connection requests
    err = netconn_listen(conn);

    if (err != ERR_OK)
    {
    	PRINTF("ERROR: WF200_HandleNetworkDetailsInput - netconn_listen error");
    	WF200_DriverStatus = SL_STATUS_FAIL;

    	WF200_DEBUG_TRAP;
    }

    /* Block until we get an incoming connection */
	err = netconn_accept(conn, &newconn);

    if (err != ERR_OK)
    {
        PRINTF("ERROR: WF200_HandleNetworkDetailsInput - netconn_accept error");
    	WF200_DriverStatus = SL_STATUS_FAIL;

    	 WF200_DEBUG_TRAP;
    }

    err = netconn_recv(newconn, &rxBuffer);

    if (err != ERR_OK)
    {
        PRINTF("ERROR: WF200_HandleNetworkDetailsInput - netconn_recv error");
        WF200_DriverStatus = SL_STATUS_FAIL;

        WF200_DEBUG_TRAP;
    }

    if(WF200_DriverStatus == SL_STATUS_OK)
    {
    	netbuf_data(rxBuffer, &data, &len);
	    memcpy(datax, data, len);

	    PRINTF("\nReceived data: len = %i\n%s\n", len, (char*) datax);

		PRINTF("!  # Ch RSSI MAC (BSSID)        Network (SSID)\r\n");

		// Carry out a scan to report to the App
		WF200_DriverStatus = sl_wfx_send_scan_command(WFM_SCAN_MODE_ACTIVE, NULL, 0, NULL, 0, NULL, 0, NULL);


		if((WF200_DriverStatus != SL_STATUS_OK) && (WF200_DriverStatus != SL_STATUS_WIFI_WARNING))
		{
	    	PRINTF("ERROR: WF200_HandleNetworkDetailsInput - scan command error");
	    	WF200_DriverStatus = SL_STATUS_FAIL;

	    	WF200_DEBUG_TRAP;
		}

		scan_ongoing = true;

	    while((scan_ongoing == true) && (timeout != 100))
	    {
	    	vTaskDelay(100);
	    	timeout++;
	    }

	    if(scan_ongoing == true)
	    {
	    	PRINTF("ERROR: WF200_HandleNetworkDetailsInput - scan timeout");
	    	WF200_DriverStatus = SL_STATUS_FAIL;

	    	WF200_DEBUG_TRAP;
	    }
    }

	int16_t rssi;
	uint16_t startIndex = WF200_SCAN_RESULT_01_CHANNEL_OFFSET;
	char OutputBuffer[4];
	uint16_t temp;
	uint8_t num;

	for(uint8_t numScanResults=0; numScanResults<scan_count_web; numScanResults++)
	{
		sprintf(&OutputBuffer[0], "%i", scan_list[numScanResults].channel);

		temp = startIndex;
		num = 2;

		if(scan_list[numScanResults].channel < 9)
		{
			temp = temp+1;
			num = 1;
		}

		memcpy(&WebPage[temp],
			   &OutputBuffer[0],
			   num);

	    rssi = ((int16_t)(scan_list[numScanResults].rcpi - 220) / 2),

		sprintf(&OutputBuffer[0], "%i", rssi);
		memcpy(&WebPage[startIndex+WF200_SCAN_RESULT_01_RSSI_OFFSET],
			   &OutputBuffer[0],
			   4);

		uint8_t offset = 0;

		for(uint8_t numMac=0; numMac<6; numMac++)
		{
			sprintf(&OutputBuffer[0], "%x", scan_list[numScanResults].mac[numMac]);

			memcpy(&WebPage[startIndex+WF200_SCAN_RESULT_01_MAC_OFFSET+offset],
				   &OutputBuffer[0],
				   2);

			offset += 3;
		}

		memcpy(&WebPage[startIndex+WF200_SCAN_RESULT_01_NETWORK_OFFSET],
			   &scan_list[numScanResults].ssid_def.ssid,
			   scan_list[numScanResults].ssid_def.ssid_length);


		startIndex += WF200_SCAN_RESULT_BLOCK_SIZE;
	}


	if (err == ERR_OK)
	{
		netconn_write(newconn, WebPage, sizeof(WebPage), NETCONN_COPY);
	}

// !!! TO BE COMPLETED !!!
//	while(idNotFound == 0)
//	{
		/* block until we get an incoming connection */
// 	    err = netconn_accept(conn, &newconn);


//	    if((err = netconn_recv(newconn, &rxBuffer)) == ERR_OK)
//	    {
//	    	netbuf_data(rxBuffer, &data, &len);

//		    memcpy(datax, data, len);

//		    PRINTF("\nReceived data: %i \n%s\n", len, (char*) datax);
//		    netconn_write(newconn, WebPageResponse, sizeof(WebPageResponse), NETCONN_COPY);

	///	idNotFound = 1;
//	    }
//	}


//    while(idNotFound == 0) /*uint8_t outerloop=0; outerloop < 8; outerloop++*/
//    {
//    	/* block until we get an incoming connection */
//    	err = netconn_accept(conn, &newconn);
//        if (err == ERR_OK)
//        {
//        	if((err = netconn_recv(newconn, &rxBuffer)) == ERR_OK)
//        	{
//
//
//        		for(uint16_t loopCheck=0; loopCheck<len-10; loopCheck++)
//        		{
//        			if( (datax[loopCheck] == 'n') && (datax[loopCheck+1] == 'a') &&
//						(datax[loopCheck+2] == 'm') && (datax[loopCheck+3] == 'e'))
//        			{
//        				for(uint16_t copyLoop=0; copyLoop<9; copyLoop++)
//        				{
//        					wlan_ssid[copyLoop] = datax[(loopCheck+5+copyLoop)];
//        					password[copyLoop] = datax[(loopCheck+20+copyLoop)];
//
//        				}
//
//        				PRINTF("\nNetwork Info: SSID: %s    PASSWORD: %s\n", wlan_ssid, password);
//
//        				idNotFound = 1;
//        				break;
//        			}
//
//        		}
//
//        		if(WriteCount == 0)
//        		{
//        			netconn_write(newconn, text, sizeof(text), NETCONN_COPY);
//        		    PRINTF("Data Output:\n%.*s\n", sizeof(text), (char*) text);
//        		    WriteCount++;
//        		}
//        		else
//        		{
//        			netconn_write(newconn, textxxx, sizeof(textxxx), NETCONN_COPY);
//        			PRINTF("Data Output:\n%.*s\n", sizeof(text), (char*) text);
//        		}
//
//
//        	}



//        }

        /* deallocate both connections */

//    }


    /// NetworkDetailsKnown = true; // ????????????????????????????????????????????
	netconn_delete(newconn);
	netbuf_delete(rxBuffer);
    netconn_delete(conn);

    /////// !!!!! ENSURE AT THIS STAGE THE SSID, PASSKEY AND SECURITY TYPE OF THE NETOWRK RO CONNECT TO IS SET !!!!! ///////

	return SL_STATUS_OK; // WF200_DriverStatus ;
}
