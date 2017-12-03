/************************************/
/*  nyuukai.c                       */
/*    入会登録処理プログラム        */
/************************************/
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "main.h"
#include "nyuukai.h"

extern int akicode_tbl[ MEMBER_MAX + 1 ];         /* 空きコード表            */
extern int codedata_tbl[ MEMBER_MAX ];
extern struct KEISOKU_TBL kojin_keisoku_tbl;

/***********************************/
/* 入会登録処理                    */
/*   メインルーチン                */
/*                                 */
/*   パラメータ : なし             */
/*   リターン   : 0:OK             */
/*               -1:NG             */
/***********************************/
int nyuukai_touroku( void )
{
    int    ret;
    int    kaiin_code;
    long   fptr;
    char   msg[ 64 ];

    if( (ret = akicode_tbl_read( )) == NG ) {
        return ret;
    }

    if( akicode_tbl[ 0 ] <= 0 ) {
        printf( "\n 残念ながらただ今メンバーの空きがありません\n" );
        ret = OK;
        return ret;
    }

    sprintf( msg, "\n 会員コードは %d です。よろしいですか( Y/N )", akicode_tbl[ 1 ] );

    if( (ret = kakunin_input( msg )) == OK ) {
        kaiin_code = akicode_tbl[ 1 ];

        if( (ret = akicode_tbl_update( )) == OK ) {

            if( (ret = keisoku_tbl_add( &fptr, kaiin_code )) == OK ) {

                ret = codedata_tbl_update( kaiin_code, fptr );
            }
        }
    }

    if( ret == OK ) {
        printf( "\n 入会登録処理が終了しました" );
    }

    return ret;
}



static int akicode_tbl_update( void )
{
    int     ret;
    int     i;
    int     cnt;
    FILE    *fp;
    char    *fname = AKICODE_TBL_NAME;

    cnt = akicode_tbl[ 0 ];

    for( i = 1; i < cnt; i++ ) {
        if( akicode_tbl[ i + 1 ] == 0 ) {
            break;
        }
        akicode_tbl[ i ] = akicode_tbl[ i + 1 ];
    }

    akicode_tbl[ i ] = 0;

    akicode_tbl[ 0 ] = cnt - 1;

    if( (fp = fopen( fname, "w+b" )) == NULL ) {
        printf( "\n 空きコード表ファイル OPEN エラー" );
        return NG;
    }

    if( (ret = fwrite( (char *)akicode_tbl,
               sizeof( int ) * (akicode_tbl[ 0 ] + 1), 1, fp )) != 1 ) {
        printf( "\n 空きコード表ファイル WRITE エラー" );
        ret = NG;
    }
    else {
        ret = OK;
    }

    fclose( fp );

    return ret;
}



static int keisoku_tbl_add( long *fptr, int kaiin_code )
{
    int     ret;
    FILE    *fp;
    char    *fname = KEISOKU_TBL_NAME;

    if( (fp = fopen( fname, "a+b" )) == NULL ) {
        printf( "\n 計測データ表ファイル OPEN エラー" );
        return NG;
    }

    if( (ret = fseek( fp, 0L, SEEK_END )) != OK ) {
        printf( "\n 計測データ表ファイル SEEK エラー" );
        fclose( fp );
        return NG;
    }

    *fptr = ftell( fp );

    kojin_keisoku_tbl = init_kojin_keisoku_tbl();

    kojin_keisoku_tbl.kaiin_code = kaiin_code;

    if( (ret = fwrite( (char *)&kojin_keisoku_tbl, sizeof( kojin_keisoku_tbl ),
               1, fp )) != 1 ) {
        printf( "\n 計測データ表ファイル WRITE エラー" );
        ret = NG;
    }
    else {
        ret = OK;
    }

    fclose( fp );

    return ret;
}



static int codedata_tbl_update( int kaiin_code, long fptr )
{
    int     ret;
    FILE    *fp;
    char    *fname = CODEDATA_TBL_NAME;

    if( (fp = fopen( fname, "r+b" )) == NULL ) {
        printf( "\n コード・データ対照表ファイル OPEN エラー" );
        return NG;
    }

    if( (ret = fread( (char *)codedata_tbl, sizeof( codedata_tbl ), 1, fp ) )
               != 1 ) {
        printf( "\n コード・データ対照表ファイル READ エラー" );
        ret = NG;
    }
    else {
        codedata_tbl[ kaiin_code - 1 ] = 
            (int)( (fptr / sizeof( struct KEISOKU_TBL )) + 1 );

        /* ファイルポインタを先頭に SEEK -> OK でない ? */
        if( (ret = fseek( fp, 0L, SEEK_SET )) != OK ) {
            printf( "\n コード・データ対照表ファイル SEEK エラー" );

            fclose( fp );
            return NG;
        }

        if( (ret = fwrite( (char *)codedata_tbl, sizeof( codedata_tbl ), 1,
                   fp )) != 1 ) {
            printf( "\n コード・データ対照表ファイル WRITE エラー" );
            ret = NG;
        }
        else {
            ret = OK;
        }
    }

    fclose( fp );

    return ret;
}
