/************************************/
/*  sakujyo.c                       */
/*    登録削除処理プログラム        */
/************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "main.h"
#include "sakujyo.h"

extern int akicode_tbl[ MEMBER_MAX + 1 ];         /* 空きコード表            */
extern int codedata_tbl[ MEMBER_MAX ];
extern struct KEISOKU_TBL kojin_keisoku_tbl;

/************************************/
/* 登録削除処理                     */
/*   メインルーチン                 */
/*                                  */
/*   パラメータ : なし              */
/*   リターン   : 0:OK              */
/*               -1:NG              */
/************************************/
int touroku_sakujyo( void )
{
    int     ret;
    int     loop = TRUE;
    int     kaiin_code;
    char    work[ 128 ];

    while( loop ) {
        printf( "\n 退会者の会員コードを入力してください" );
        printf( "\n ? " );

        work[ 0 ] = '\0';
        scanf( "%s", work );

        if( strspn( work, "1234567890" ) < strlen( work ) ) {
            printf( "\n 数値以外が入力されました" );
            continue;
        }

        kaiin_code = atoi( work );
        if( kaiin_code > MEMBER_MAX || kaiin_code <= 0 ) {
            printf( "\n 入力ミスです" );
            continue;
        }

        if( (ret = codedata_tbl_delete( kaiin_code )) == OK ) {
            if( (ret = akicode_tbl_add( kaiin_code )) == OK ) {
                loop = FALSE;
            }
        }
        else {
            loop = FALSE;
        }
    }

    if( ret == OK ) {
        printf( "\n 入会登録削除処理が終了しました" );
    }

    return ret;
}



static int codedata_tbl_delete( int kaiin_code )
{
    int     ret;
    int     i;
    char    msg[ 64 ];
    FILE    *fp;
    char    *fname = CODEDATA_TBL_NAME;

    if( (fp = fopen( fname, "r+b" )) == NULL ) {
        printf( "\n コード・データ対照表ファイル OPEN エラー" );
        return NG;
    }

    if( (ret = fread( (char *)codedata_tbl, sizeof( codedata_tbl ), 1, fp ) )
               != 1 ) {
        printf( "\n コード・データ対照表ファイル READ エラー" );

        fclose( fp );
        return NG;
    }

    if( codedata_tbl[ kaiin_code - 1 ] == 0 ) {
        printf( "\n この会員コードは未登録です" );

        fclose( fp );
        return CANCEL;
    }

    if( (ret = kojin_data_read( kaiin_code )) == NG ) {
        /* コード・データ対照表ファイル CLOSE */
        fclose( fp );
        return ret;
    }

    kojin_data_disp( kaiin_code, "\n ** 削除データ **" );

    sprintf( msg, "\n\n 上のデータを削除します。よろしいですか( Y/N )" );

    if( (ret = kakunin_input( msg )) == OK ) {

        if( (ret = kojin_data_delete( kaiin_code )) == OK ) {

            for( i = 0; i < MEMBER_MAX; i++ ) {
                if( codedata_tbl[ i ] > codedata_tbl[ kaiin_code - 1 ] ) {
                    codedata_tbl[ i ]--;
                }
            }

            codedata_tbl[ kaiin_code - 1 ] = 0;

            if( (ret = fseek( fp, 0L, SEEK_SET )) == OK ) {

                if( (ret = fwrite( (char *)codedata_tbl,
                           sizeof( codedata_tbl ),1, fp )) != 1 ) {
                    printf( "\n コード・データ対照表ファイル WRITE エラー" );
                    ret = NG;
                }
                else {
                    ret = OK;
                }
            }
            else {
                printf( "\n コード・データ対照表ファイル SEEK エラー" );
                ret = NG;
            }
        }
    }
    else {
        ret = CANCEL;
    }

    fclose( fp );

    return ret;
}



static int kojin_data_delete( int kaiin_code )
{
    int     ret;
    int     i;
    FILE    *fp;
    FILE    *tmp;
    char    *fname = KEISOKU_TBL_NAME;
    char    *tmpfl = "keisoku.tmp";

    if( (fp = fopen( fname, "rb" )) == NULL ) {
        printf( "\n 計測データ表ファイル OPEN エラー" );
        return NG;
    }

    if( (tmp = fopen( tmpfl, "w+b" )) == NULL ) {
        printf( "\n テンポラリファイル OPEN エラー" );
        fclose( fp );
        return NG;
    }

    i = 0;
    for( ; ; ) {

        if( (ret = fread( (char *)&kojin_keisoku_tbl,
                   sizeof( kojin_keisoku_tbl ), 1, fp )) != 1 ) {

            if( ferror( fp ) != 0 ) {
                printf( "\n 計測データ表ファイル READ エラー" );
                ret = NG;
            }
            else {
                if( feof( fp ) == 0 ) {
                    printf( "\n 計測データ表ファイル READ エラー" );
                    ret = NG;
                }
                else {
                   ret = OK;
                }
            }
            break;
        }

        if( kaiin_code == kojin_keisoku_tbl.kaiin_code ) {
            continue;
        }

        if( (ret = fwrite( (char *)&kojin_keisoku_tbl,
                   sizeof( kojin_keisoku_tbl ), 1, tmp )) != 1 ) {
            printf( "\n 計測データ表ファイル WRITE エラー" );
            ret = NG;
            break;
        }
        i++;
    }

    fclose( tmp );

    fclose( fp );

    if( ret == OK ) {

        if( (ret = remove( fname )) != 0 ) {
            printf( "\n 計測データ表ファイル削除エラー" );
            ret = NG;
        }
        else {
            if( i > 0 ) {

                if( (ret = rename( tmpfl, fname )) != 0 ) {
                    printf( "\n 計測データ表ファイルリネームエラー" );
                    ret = NG;
                }
            }
            else {
                remove( tmpfl );
            }
        }
    }
    else {
        remove( tmpfl );
    }

    return ret;
}



static int akicode_tbl_add( int kaiin_code )
{
    int     ret;
    int     cnt;
    FILE    *fp;
    char    *fname = AKICODE_TBL_NAME;

    if( (ret = akicode_tbl_read( )) == NG ) {
        return ret;
    }

    cnt = akicode_tbl[ 0 ];

    akicode_tbl[ cnt + 1 ] = kaiin_code;

    akicode_tbl[ 0 ] = cnt + 1;

    if( (fp = fopen( fname, "w+b" )) == NULL ) {
        printf( "\n 空きコード表ファイル OPEN エラー" );
        return NG;
    }

    if( (ret = fwrite( (char *)akicode_tbl,
               sizeof( int ) * ( akicode_tbl[ 0 ] + 1 ), 1, fp )) != 1 ) {
        printf( "\n 空きコード表ファイル WRITE エラー" );
        ret = NG;
    }
    else {
        ret = OK;
    }

    fclose( fp );

    return ret;
}
