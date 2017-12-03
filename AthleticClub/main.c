/*******************************************/
/*  main.c                                 */
/*    アスレチッククラブ会員管理プログラム */
/*******************************************/

#include <stdio.h>
#include <string.h>

#include "common.h"
#include "main.h"
#include "nyuukai.h"
#include "keisoku.h"
#include "sakujyo.h"

/***********************************/
/* 共通データ                      */
/***********************************/
int akicode_tbl[ MEMBER_MAX + 1 ];                /* 空きコード表            */
int codedata_tbl[ MEMBER_MAX ];
struct KEISOKU_TBL kojin_keisoku_tbl;
struct KEISOKU_TBL sort_keisoku_tbl[ MEMBER_MAX ];


/***********************************/
/* 会員管理メイン制御              */
/*   メインルーチン                */
/*                                 */
/*   パラメータ : なし             */
/*   リターン   : なし             */
/***********************************/
int main( void )
{
    int    loop = TRUE;
    char   work[ 128 ];
    FILE   *fp;
    char   *fname1 = AKICODE_TBL_NAME;
    char   *fname2 = CODEDATA_TBL_NAME;
    int    i;

    if( (fp = fopen( fname1, "rb" )) == NULL ) {
        printf( "\n 空きコード表ファイルを作成しています" );

        akicode_tbl_create( );
    }
    else {
        fclose( fp );
    }

    if( (fp = fopen( fname2, "rb" )) == NULL ) {
        printf( "\n コード・データ対照表ファイルを作成しています" );

        codedata_tbl_create( );
    }
    else {
        fclose( fp );
    }

    while( loop ) {
        akicode_tbl[ 0 ] = 0;
        for( i = 0; i < MEMBER_MAX; i++) {
            akicode_tbl[ i + 1 ] = 0;
            codedata_tbl[ i ] = 0;
            sort_keisoku_tbl[ i ] = init_kojin_keisoku_tbl();
        }
        kojin_keisoku_tbl = init_kojin_keisoku_tbl();

        printf( "\n" );
        printf( "\n *******************************************" );
        printf( "\n  アスレチッククラブ メンバー管理プログラム" );
        printf( "\n *******************************************" );
        printf( "\n 処理を選択してください" );
        printf( "\n 1:入会登録" );
        printf( "\n 2:計測記録入力" );
        printf( "\n 3:登録削除" );
        printf( "\n E:終了" );
        printf( "\n ? " );

        work[ 0 ] = '\0';
        scanf( "%s", work );

        if( strlen( work ) != 1 ) {
            printf( "\n 入力ミスです" );
            continue;
        }

        switch( work[ 0 ] ) {
            case '1':
                nyuukai_touroku( );
                break;

            case '2':
                keisoku_input( );
                break;

            case '3':
                touroku_sakujyo( );
                break;

            case 'e':
            case 'E':
                loop = FALSE;
                break;

            default:
                printf( "\n 入力ミスです" );
                break;
        }
    }
    return OK;
}



static int akicode_tbl_create( void )
{
    int     ret;
    int     i;
    FILE    *fp;
    char    *fname = AKICODE_TBL_NAME;

    akicode_tbl[ 0 ] = MEMBER_MAX;
    for( i = 1; i < MEMBER_MAX + 1; i++ ) {
        akicode_tbl[ i ] = i;
    }

    if( (fp = fopen( fname, "w+b" )) == NULL ) {
        printf( "\n 空きコード表ファイル OPEN エラー" );
        return NG;
    }

    if( (ret = fwrite( (char *)akicode_tbl, sizeof( akicode_tbl ), 1, fp ) )
               != 1 ) {
        printf( "\n 空きコード表ファイル WRITE エラー" );
        ret = NG;
    }
    else {
        ret = OK;
    }

    fclose( fp );

    return ret;
}



static int codedata_tbl_create( void )
{
    int     ret;
    FILE    *fp;
    char    *fname = CODEDATA_TBL_NAME;
    int     i;

    if( (fp = fopen( fname, "w+b" )) == NULL ) {
        printf( "\n コード・データ対照表ファイル OPEN エラー" );
        return NG;
    }

    for( i = 0; i < MEMBER_MAX; i++ )
        codedata_tbl[ i ] = 0;

    if( (ret = fwrite( (char *)codedata_tbl, sizeof( codedata_tbl ), 1, fp ) )
               != 1 ) {
        printf( "\n コード・データ対照表ファイル WRITE エラー" );
        ret = NG;
    }
    else {
        ret = OK;
    }

    fclose( fp );

    return ret;
}



int akicode_tbl_read( void )
{
    int     ret;
    int     i;
    FILE    *fp;
    char    *fname = AKICODE_TBL_NAME;

    if( (fp = fopen( fname, "rb" )) == NULL ) {
        printf( "\n 空きコード表ファイル OPEN エラー" );
        return NG;
    }

    for( i = 0; i < MEMBER_MAX + 1; i++ ) {
        if( (ret = fread( (char *)&akicode_tbl[ i ], sizeof( int ), 1, fp ) )
                   != 1 ) {
            if( ferror( fp ) != 0 ) {
                printf( "\n 空きコード表ファイル READ エラー" );
                ret = NG;
            }
            else {
                if( feof( fp ) == 0 ) {
                    printf( "\n 空きコード表ファイル READ エラー" );
                    ret = NG;
                }
                else {
                    ret = OK;
                }
            }
            break;
        }
    }

    fclose( fp );

    return ret;
}



int kakunin_input( char *msg )
{
    int     ret;
    int     loop = TRUE;
    char    work[ 128 ];

    while( loop ) {
        printf( msg );
        printf( "\n ? " );

        work[ 0 ] = '\0';
        scanf( "%s", work );

        if( strlen( work ) != 1 ) {
            printf( "\n 入力ミスです" );
            continue;
        }

        switch( work[ 0 ] ) {
            case 'Y':
            case 'y':
                ret  = OK;
                loop = FALSE;
                break;

            case 'N':
            case 'n':
                ret  = NG;
                loop = FALSE;
                break;

            default:
                printf( "\n 入力ミスです" );
                break;
        }
    }

    return ret;
}



int kojin_data_read( int kaiin_code )
{
    int     ret;
    FILE    *fp;
    long    fptr;
    char    *fname = KEISOKU_TBL_NAME;

    if( (fp = fopen( fname, "rb" )) == NULL ) {
        printf( "\n 計測データ表ファイル OPEN エラー" );
        return NG;
    }

    fptr = ( codedata_tbl[ kaiin_code - 1 ] - 1 ) *
             sizeof( struct KEISOKU_TBL );

    if( (ret = fseek( fp, fptr, SEEK_SET )) != OK ) {
        printf( "\n 計測データ表ファイル SEEK エラー" );

        fclose( fp );
        return NG;
    }

    if( (ret = fread( (char *)&kojin_keisoku_tbl, sizeof( kojin_keisoku_tbl ),
               1, fp )) != 1 ) {
        printf( "\n 計測データ表 READ エラー" );
        ret = NG;
    }
    else {
        ret = OK;
    }

    fclose( fp );

    return ret;
}



void kojin_data_disp( int kaiin_code, char *msg )
{
    printf( msg );
    printf( "\n 会員コード %3d", kaiin_code );

    if( kojin_keisoku_tbl.count != 0 ) {
        printf( "\n\n 計測回数    初回日付  データ    " );
        printf( " 最高日付  データ     最新日付  データ" );

        printf( "\n    %3d", kojin_keisoku_tbl.count );

        printf( "    %4.4s-%2.2s-%2.2s",
            &kojin_keisoku_tbl.first_date[ 0 ],
            &kojin_keisoku_tbl.first_date[ 4 ],
            &kojin_keisoku_tbl.first_date[ 6 ] );

        printf( "   %4d", kojin_keisoku_tbl.first_data );

        printf( "    %4.4s-%2.2s-%2.2s",
            &kojin_keisoku_tbl.max_date[ 0 ],
            &kojin_keisoku_tbl.max_date[ 4 ],
            &kojin_keisoku_tbl.max_date[ 6 ] );

        printf( "   %4d", kojin_keisoku_tbl.max_data );

        printf( "    %4.4s-%2.2s-%2.2s",
            &kojin_keisoku_tbl.soku_date[ 0 ],
            &kojin_keisoku_tbl.soku_date[ 4 ],
            &kojin_keisoku_tbl.soku_date[ 6 ] );

        printf( "   %4d", kojin_keisoku_tbl.soku_data[ 0 ] );

        printf( "\n\n    1回前   2回前   3回前   4回前   ");
        printf( "5回前   6回前   7回前   8回前   9回前" );

        printf( "\n " );
        printf( "    %4d", kojin_keisoku_tbl.soku_data[ 1 ] );
        printf( "    %4d", kojin_keisoku_tbl.soku_data[ 2 ] );
        printf( "    %4d", kojin_keisoku_tbl.soku_data[ 3 ] );
        printf( "    %4d", kojin_keisoku_tbl.soku_data[ 4 ] );
        printf( "    %4d", kojin_keisoku_tbl.soku_data[ 5 ] );
        printf( "    %4d", kojin_keisoku_tbl.soku_data[ 6 ] );
        printf( "    %4d", kojin_keisoku_tbl.soku_data[ 7 ] );
        printf( "    %4d", kojin_keisoku_tbl.soku_data[ 8 ] );
        printf( "    %4d", kojin_keisoku_tbl.soku_data[ 9 ] );
    }
    else {
        printf( "    計測データがありません" );
        return;
    }
    return;
}



struct KEISOKU_TBL init_kojin_keisoku_tbl( void )
{
    static struct KEISOKU_TBL tbl = {
        0, 0, "        ", 0, "        ", 0, "        ", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    return tbl;
}
