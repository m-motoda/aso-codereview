/*******************************************/
/*  main.c                                 */
/*    �A�X���`�b�N�N���u����Ǘ��v���O���� */
/*******************************************/

#include <stdio.h>
#include <string.h>

#include "common.h"
#include "main.h"
#include "nyuukai.h"
#include "keisoku.h"
#include "sakujyo.h"

/***********************************/
/* ���ʃf�[�^                      */
/***********************************/
int akicode_tbl[ MEMBER_MAX + 1 ];                /* �󂫃R�[�h�\            */
int codedata_tbl[ MEMBER_MAX ];
struct KEISOKU_TBL kojin_keisoku_tbl;
struct KEISOKU_TBL sort_keisoku_tbl[ MEMBER_MAX ];


/***********************************/
/* ����Ǘ����C������              */
/*   ���C�����[�`��                */
/*                                 */
/*   �p�����[�^ : �Ȃ�             */
/*   ���^�[��   : �Ȃ�             */
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
        printf( "\n �󂫃R�[�h�\�t�@�C�����쐬���Ă��܂�" );

        akicode_tbl_create( );
    }
    else {
        fclose( fp );
    }

    if( (fp = fopen( fname2, "rb" )) == NULL ) {
        printf( "\n �R�[�h�E�f�[�^�Ώƕ\�t�@�C�����쐬���Ă��܂�" );

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
        printf( "\n  �A�X���`�b�N�N���u �����o�[�Ǘ��v���O����" );
        printf( "\n *******************************************" );
        printf( "\n ������I�����Ă�������" );
        printf( "\n 1:����o�^" );
        printf( "\n 2:�v���L�^����" );
        printf( "\n 3:�o�^�폜" );
        printf( "\n E:�I��" );
        printf( "\n ? " );

        work[ 0 ] = '\0';
        scanf( "%s", work );

        if( strlen( work ) != 1 ) {
            printf( "\n ���̓~�X�ł�" );
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
                printf( "\n ���̓~�X�ł�" );
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
        printf( "\n �󂫃R�[�h�\�t�@�C�� OPEN �G���[" );
        return NG;
    }

    if( (ret = fwrite( (char *)akicode_tbl, sizeof( akicode_tbl ), 1, fp ) )
               != 1 ) {
        printf( "\n �󂫃R�[�h�\�t�@�C�� WRITE �G���[" );
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
        printf( "\n �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� OPEN �G���[" );
        return NG;
    }

    for( i = 0; i < MEMBER_MAX; i++ )
        codedata_tbl[ i ] = 0;

    if( (ret = fwrite( (char *)codedata_tbl, sizeof( codedata_tbl ), 1, fp ) )
               != 1 ) {
        printf( "\n �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� WRITE �G���[" );
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
        printf( "\n �󂫃R�[�h�\�t�@�C�� OPEN �G���[" );
        return NG;
    }

    for( i = 0; i < MEMBER_MAX + 1; i++ ) {
        if( (ret = fread( (char *)&akicode_tbl[ i ], sizeof( int ), 1, fp ) )
                   != 1 ) {
            if( ferror( fp ) != 0 ) {
                printf( "\n �󂫃R�[�h�\�t�@�C�� READ �G���[" );
                ret = NG;
            }
            else {
                if( feof( fp ) == 0 ) {
                    printf( "\n �󂫃R�[�h�\�t�@�C�� READ �G���[" );
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
            printf( "\n ���̓~�X�ł�" );
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
                printf( "\n ���̓~�X�ł�" );
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
        printf( "\n �v���f�[�^�\�t�@�C�� OPEN �G���[" );
        return NG;
    }

    fptr = ( codedata_tbl[ kaiin_code - 1 ] - 1 ) *
             sizeof( struct KEISOKU_TBL );

    if( (ret = fseek( fp, fptr, SEEK_SET )) != OK ) {
        printf( "\n �v���f�[�^�\�t�@�C�� SEEK �G���[" );

        fclose( fp );
        return NG;
    }

    if( (ret = fread( (char *)&kojin_keisoku_tbl, sizeof( kojin_keisoku_tbl ),
               1, fp )) != 1 ) {
        printf( "\n �v���f�[�^�\ READ �G���[" );
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
    printf( "\n ����R�[�h %3d", kaiin_code );

    if( kojin_keisoku_tbl.count != 0 ) {
        printf( "\n\n �v����    ������t  �f�[�^    " );
        printf( " �ō����t  �f�[�^     �ŐV���t  �f�[�^" );

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

        printf( "\n\n    1��O   2��O   3��O   4��O   ");
        printf( "5��O   6��O   7��O   8��O   9��O" );

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
        printf( "    �v���f�[�^������܂���" );
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
/*�ȏ�ő��Ǝ����I���������܂��B*/