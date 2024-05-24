#ifndef CALIBRATE_H_
#define CALIBRATE_H_

#ifdef __cplusplus
 extern "C" {
#endif

/****************************************************/
/*                                                  */
/* Included files                                   */
/*                                                  */
/****************************************************/



/****************************************************/
/*                                                  */
/* Definitions                                      */
/*                                                  */
/****************************************************/

#ifndef         _CALIBRATE_C_
        #define         EXTERN         extern
#else
        #define         EXTERN
#endif



#ifndef         OK
        #define         OK                      0
        #define         NOT_OK             -1
#endif



#define                 INT_32                           long




/****************************************************/
/*                                                  */
/* Structures                                       */
/*                                                  */
/****************************************************/


typedef struct Point {
                        INT_32    x,
                                 y ;
                     } POINT_T ;



typedef struct Matrix {
                                                        /* This arrangement of values facilitates
                                                         *  calculations within getDisplayPoint()
                                                         */
                        INT_32    An,     /* A = An/Divider */
                                 Bn,     /* B = Bn/Divider */
                                 Cn,     /* C = Cn/Divider */
                                 Dn,     /* D = Dn/Divider */
                                 En,     /* E = En/Divider */
                                 Fn,     /* F = Fn/Divider */
                                 Divider ;
                     } MATRIX ;




/****************************************************/
/*                                                  */
/* Function declarations                            */
/*                                                  */
/****************************************************/


EXTERN int setCalibrationMatrix( POINT_T * display,
								 POINT_T * screen,
                                 MATRIX * matrix) ;


EXTERN int getDisplayPoint( POINT_T * display,
							POINT_T * screen,
                            MATRIX * matrix ) ;

#ifdef __cplusplus
}
#endif

#endif  /* CALIBRATE_H_ */
