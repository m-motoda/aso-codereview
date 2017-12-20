/********************************************************************************
*画像回転プログラム
********************************************************************************/
#include<stdio.h>
int rotation_output(int[10][10]);
int rotation_input(int Direction);
int rotation(int [10][10],int Direction);
/********************************************************************************
*int main(void)
*メイン処理
*・パラメータ値 なし
*・リターン値  なし
********************************************************************************/
int main(void){
  int i,j;                                                /*画像の座標*/
  int Direction;                                          /*回転値*/
  char dmy[80];                                           /*入力用配列*/
  int ret=1;                                              /*および関数*/
  int image[10][10]={{1,0,0,0,0,0,0,0,0,0},               /*画像配列*/
                     {1,1,0,0,0,0,0,0,0,0},               /*( 1)=■*/
                     {1,0,1,0,0,0,0,0,0,0},               /*( 0)=□*/
                     {1,0,0,1,0,0,0,0,0,0},
                     {1,0,0,0,1,0,0,0,0,0},
                     {1,0,0,0,0,1,0,0,0,0},
                     {1,0,0,0,0,0,1,0,0,0},
                     {1,0,0,0,0,0,0,1,0,0},
                     {1,0,0,0,0,0,0,0,1,0},
                     {1,0,0,0,0,0,0,0,0,1}};
  printf("画像回転プログラム\n");
  for(;;){
    rotation_output(image);                               /*画像表示処理*/
    do{
      printf("入力してください(( 1)右回転/(-1)左回転)):");
      ret = scanf("%d",&Direction);                       /*回転値入力*/
      if(ret != 1) gets(dmy);                             /*不正入力対策*/
      rotation_input(Direction);                          /*回転値判断・指定*/
    }while((Direction!=1)&&(Direction!=-1));              /*不正な回転値のみループ*/
    rotation(image,Direction);                            /*回転処理*/
    Direction=0;                                          /*値継続防止の為初期化*/
  }
}
/********************************************************************************
*int spin_output(int)
*画像表示処理
*・パラメータ値  int image[10][10], int Direction
*・リターン値  なし
********************************************************************************/
int rotation_output(int image[10][10]){
  int i,j;
  for(i=0;i<10;i++){
    for(j=0;j<10;j++){
      if(image[i][j]==1){                                 /*1の場合*/
        printf("■");
      }else{                                              /*0の場合*/
        printf("□");
      }
    }
    printf("\n");
  }
return 0;
}
/********************************************************************************
*int spin_input(int)
*回転値判断・指定
*・パラメータ値  int Direction
*・リターン値  なし
********************************************************************************/
int rotation_input(int Direction){
  if(Direction==1){
    puts("右方向へ90°回転\n");
  }else if(Direction==-1){
    printf("左方向へ90°回転\n");
  }else{
    printf("不正な入力値です。\n");
  }
return 0;
}
/********************************************************************************
*int spin(int)
*画像回転処理
*・パラメータ値  int image[10][10], int Direction
*・リターン値  なし
********************************************************************************/
int rotation(int image[10][10],int Direction){
  int i,j;                                                /*配列の座標*/
  int max=9;                                              /*最大値*/
  int start=0;                                            /*探索開始の値*/
  int shuffle[4];                                         /*回転用配列*/

  for(i=0;i<5;i++){
    for(j=start;j<max-i;j++){
      shuffle[0]=image[i][j];                             /*左上配列格納*/
      shuffle[1]=image[j][max-i];                         /*右上配列格納*/
      shuffle[2]=image[max-i][max-j];                     /*右下配列格納*/
      shuffle[3]=image[max-j][i];                         /*左下配列格納*/
      if(Direction==1){
        image[j][max-i]=shuffle[0];                       /*右上←左上*/
        image[max-i][max-j]=shuffle[1];                   /*右下←右上*/
        image[max-j][i]=shuffle[2];                       /*左下←右下*/
        image[i][j]=shuffle[3];                           /*左上←左下*/
      }else if(Direction==-1){
        image[j][max-i]=shuffle[2];                       /*左上←右上*/
        image[max-i][max-j]=shuffle[3];                   /*左下←左上*/
        image[max-j][i]=shuffle[0];                       /*右下←左下*/
        image[i][j]=shuffle[1];                           /*右上←右下*/
      }
    }
  start++;
  }
return 0;
}