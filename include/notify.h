/**
 * @file notify.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Mar 22 17:22:55 2012
 * @brief Notification management.
 *
 * Notification management.
 */

#ifndef _NOTIFY_H_
#define _NOTIFY_H_

#include <stdlib.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
  
  extern int verbose_level; 

  /** Purpose ID: errors */
#define FOR_ERRORS      0x00
  /** Purpose ID: warnings */
#define FOR_WARNINGS    0x01
  /** Purpose ID: information */
#define FOR_INFOS       0x02
  /** Purpose ID: debugging */
#define FOR_DEBUG       0x03

  /** Style ID: normal */
#define STYLE_OFF       0x00
  /** Style ID: bold */
#define STYLE_BOLD      0x01
  /** Style ID: underscored */
#define STYLE_USCORE    0x04
  /** Style ID: blinking */
#define STYLE_BLINK     0x05
  /** Style ID: video reverse */
#define STYLE_REVERSE   0x07
  /** Style ID: concealed */
#define STYLE_CONCEALED 0x08

  /** Color ID: black */
#define COLOR_BLACK       30
  /** Color ID: red */
#define COLOR_RED         31
  /** Color ID: green */
#define COLOR_GREEN       32
  /** Color ID: yellow */
#define COLOR_YELLOW      33
  /** Color ID: blue */
#define COLOR_BLUE        34
  /** Color ID: magenta */
#define COLOR_MAGENTA     35
  /** Color ID: cyan */
#define COLOR_CYAN        36
  /** Color ID: white */
#define COLOR_WHITE       37
  /** Color ID: default */
#define COLOR_DEFAULT     39

  /** Purpose style: errors */
#define STYLE_ERROR       STYLE_BOLD
  /** Purpose style: warnings */
#define STYLE_WARNING     STYLE_BOLD
  /** Purpose style: info */
#define STYLE_INFO        STYLE_OFF
  /** Purpose style: debugging */
#define STYLE_DEBUG       STYLE_REVERSE

  /** Purpose color: errors */
#define COLOR_ERROR       COLOR_RED
  /** Purpose color: warnings */
#define COLOR_WARNING     COLOR_BLUE
  /** Purpose color: info */
#define COLOR_INFO        COLOR_DEFAULT
  /** Purpose color: debugging */
#define COLOR_DEBUG       COLOR_MAGENTA

  /**
   * @brief Get purpose style from purpose. 
   * @param purpose Purpose. 
   */
#define STYLE(purpose)						\
  (purpose == FOR_ERRORS ? STYLE_ERROR :			\
   (purpose == FOR_WARNINGS ? STYLE_WARNING :			\
    (purpose == FOR_INFOS ? STYLE_INFO : STYLE_DEBUG )))

  /**
   * @brief Get purpose color from purpose. 
   * @param purpose Purpose. 
   */
#define COLOR(purpose)							\
  (purpose == FOR_ERRORS ? COLOR_ERROR :				\
   (purpose == FOR_WARNINGS ? COLOR_WARNING :				\
    (purpose == FOR_INFOS ? COLOR_INFO : COLOR_DEBUG )))

  /**
   * @brief Convenience macro. 
   * @param stream Output stream. 
   */
#define ON(stream) stream

  /**
   * @brief Reset color and style on output stream. 
   * @param on_stream Output stream. 
   */
#define RESET_COLORS(on_stream)			\
  fprintf( on_stream, "%c[%dm", 0x1B, 0 )

  /**
   * @brief Set color and style for given purpose on output stream. 
   * @param purpose Purpose. 
   * @param on_stream Output stream. 
   */
#define SET_COLORS(purpose, on_stream)		\
  fprintf( on_stream, "%c[%d;%dm", 0x1B,	\
	   STYLE(purpose), COLOR(purpose) )

  /**
   * @brief Report errors on @c stderr . Use it like @c printf(3) . 
   */
#define ERROR(...) do {							\
    SET_COLORS(FOR_ERRORS, ON(stderr));					\
    switch ( verbose_level ) {						\
    case 0 :								\
      fprintf( stderr, "[ ERROR ] " );					\
      break;								\
    case 1 :								\
      fprintf( stderr, "[ ERROR :: %15s:% 4d] ",			\
	       __FILE__, __LINE__ );					\
      break;								\
    default :								\
      fprintf( stderr, "[ ERROR :: %15s:%20s:% 4d] ",			\
	       __FILE__, __FUNCTION__, __LINE__ );			\
      break;								\
    }									\
    fprintf( stderr, __VA_ARGS__ );					\
    RESET_COLORS(ON(stderr));						\
} while( 0 )

  /**
   * @brief Report warnings on @c stderr . Use it like @c printf(3) . 
   */
#define WARNING(...) do {						\
    SET_COLORS(FOR_WARNINGS, ON(stderr));				\
    switch ( verbose_level ) {						\
    case 0 :								\
      fprintf( stderr, "[WARNING] " );					\
      break;								\
    case 1 :								\
      fprintf( stderr, "[WARNING:: %15s:% 4d] ",			\
	       __FILE__, __LINE__ );					\
      break;								\
    default :								\
      fprintf( stderr, "[WARNING:: %15s:%20s:% 4d] ",			\
	       __FILE__, __FUNCTION__, __LINE__ );			\
      break;								\
    }									\
    fprintf( stderr, __VA_ARGS__ );					\
    RESET_COLORS(ON(stderr));						\
} while( 0 )

  /**
   * @brief Report informations on @c stderr . Use it like @c printf(3) . 
   * @attention Only print when @ref verbose_level is strictly positive! 
   */
#define INFO(...) do {							\
    if ( 0 == verbose_level ) break;					\
    SET_COLORS(FOR_INFOS, ON(stderr));					\
    switch ( verbose_level ) {						\
    case 1 :								\
      fprintf( stderr, "[ INFO  :: %15s:% 4d] ",			\
	       __FILE__, __LINE__ );					\
      break;								\
    default :								\
      fprintf( stderr, "[ INFO  :: %15s:%20s:% 4d] ",			\
	       __FILE__, __FUNCTION__, __LINE__ );			\
      break;								\
    }									\
    fprintf( stderr, __VA_ARGS__ );					\
    RESET_COLORS(ON(stderr));						\
} while( 0 )

#ifdef NDEBUG
  /**
   * @brief Report debugging infos on @c stderr . Use it like @c printf(3) . 
   * @attention Only print when compiled in @c debug mode. 
   */
#define DEBUG(...) do {							\
    SET_COLORS(FOR_DEBUG, ON(stderr));					\
    switch ( verbose_level ) {						\
    case 0 :								\
      fprintf( stderr, "[ DEBUG ] " );					\
      break;								\
    case 1 :								\
      fprintf( stderr, "[ DEBUG :: %15s:% 4d] ",			\
	       __FILE__, __LINE__ );					\
      break;								\
    default :								\
      fprintf( stderr, "[ DEBUG :: %15s:%20s:% 4d] ",			\
	       __FILE__, __FUNCTION__, __LINE__ );			\
      break;								\
    }									\
    fprintf( stderr, __VA_ARGS__ );					\
    RESET_COLORS(ON(stderr));						\
} while( 0 )
#else
  /**
   * @brief Report debugging infos on @c stderr . Use it like @c printf(3) . 
   * @attention Only print when compiled in @c debug mode. 
   */
#define DEBUG(...)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _NOTIFY_H_ */
