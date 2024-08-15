#ifndef __TRACKERINTERFACE_H
#define __TRACKERINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif 
	/**
	 * @brief Return UART tracker signal state
	 *
	 * This function returns current state of the UART receiver signal.
	 * The receiver receives data from the plane GPS tracker
	 * as a plain text in NMEA 0183 format.
	 *
	 * UART transmission parameters:
	 * - rate: 1200 symbols / s
	 * - data bits: 8
	 * - parity bit: not included
	 * - stop bits: 2
	 *
	 * @return 0 or 1 corresponding to the current signal state.
	 */
	char get_signal_state(void);

#ifdef __cplusplus
}
#endif

#endif