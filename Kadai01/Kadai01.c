/********************************************************************************
*�摜��]�v���O����
********************************************************************************/
#include<stdio.h>
int rotation_output(int[10][10]);
int rotation_input(int Direction);
int rotation(int [10][10],int Direction);
/********************************************************************************
*int main(void)
*���C������
*�E�p�����[�^�l �Ȃ�
*�E���^�[���l  �Ȃ�
********************************************************************************/
int main(void){
  int i,j;                                                /*�摜�̍��W*/
  int Direction;                                          /*��]�l*/
  char dmy[80];                                           /*���͗p�z��*/
  int ret=1;                                              /*����ъ֐�*/
  int image[10][10]={{1,0,0,0,0,0,0,0,0,0},               /*�摜�z��*/
                     {1,1,0,0,0,0,0,0,0,0},               /*( 1)=��*/
                     {1,0,1,0,0,0,0,0,0,0},               /*( 0)=��*/
                     {1,0,0,1,0,0,0,0,0,0},
                     {1,0,0,0,1,0,0,0,0,0},
                     {1,0,0,0,0,1,0,0,0,0},
                     {1,0,0,0,0,0,1,0,0,0},
                     {1,0,0,0,0,0,0,1,0,0},
                     {1,0,0,0,0,0,0,0,1,0},
                     {1,0,0,0,0,0,0,0,0,1}};
  printf("�摜��]�v���O����\n");
  for(;;){
    rotation_output(image);                               /*�摜�\������*/
    do{
      printf("���͂��Ă�������(( 1)�E��]/(-1)����])):");
      ret = scanf("%d",&Direction);                       /*��]�l����*/
      if(ret != 1) gets(dmy);                             /*�s�����͑΍�*/
      rotation_input(Direction);                          /*��]�l���f�E�w��*/
    }while((Direction!=1)&&(Direction!=-1));              /*�s���ȉ�]�l�̂݃��[�v*/
    rotation(image,Direction);                            /*��]����*/
    Direction=0;                                          /*�l�p���h�~�̈׏�����*/
  }
}
/********************************************************************************
*int spin_output(int)
*�摜�\������
*�E�p�����[�^�l  int image[10][10], int Direction
*�E���^�[���l  �Ȃ�
********************************************************************************/
int rotation_output(int image[10][10]){
  int i,j;
  for(i=0;i<10;i++){
    for(j=0;j<10;j++){
      if(image[i][j]==1){                                 /*1�̏ꍇ*/
        printf("��");
      }else{                                              /*0�̏ꍇ*/
        printf("��");
      }
    }
    printf("\n");
  }
return 0;
}
/********************************************************************************
*int spin_input(int)
*��]�l���f�E�w��
*�E�p�����[�^�l  int Direction
*�E���^�[���l  �Ȃ�
********************************************************************************/
int rotation_input(int Direction){
  if(Direction==1){
    puts("�E������90����]\n");
  }else if(Direction==-1){
    printf("��������90����]\n");
  }else{
    printf("�s���ȓ��͒l�ł��B\n");
  }
return 0;
}
/********************************************************************************
*int spin(int)
*�摜��]����
*�E�p�����[�^�l  int image[10][10], int Direction
*�E���^�[���l  �Ȃ�
********************************************************************************/
int rotation(int image[10][10],int Direction){
  int i,j;                                                /*�z��̍��W*/
  int max=9;                                              /*�ő�l*/
  int start=0;                                            /*�T���J�n�̒l*/
  int shuffle[4];                                         /*��]�p�z��*/

  for(i=0;i<5;i++){
    for(j=start;j<max-i;j++){
      shuffle[0]=image[i][j];                             /*����z��i�[*/
      shuffle[1]=image[j][max-i];                         /*�E��z��i�[*/
      shuffle[2]=image[max-i][max-j];                     /*�E���z��i�[*/
      shuffle[3]=image[max-j][i];                         /*�����z��i�[*/
      if(Direction==1){
        image[j][max-i]=shuffle[0];                       /*�E�ど����*/
        image[max-i][max-j]=shuffle[1];                   /*�E�����E��*/
        image[max-j][i]=shuffle[2];                       /*�������E��*/
        image[i][j]=shuffle[3];                           /*���ど����*/
      }else if(Direction==-1){
        image[j][max-i]=shuffle[2];                       /*���ど�E��*/
        image[max-i][max-j]=shuffle[3];                   /*����������*/
        image[max-j][i]=shuffle[0];                       /*�E��������*/
        image[i][j]=shuffle[1];                           /*�E�ど�E��*/
      }
    }
  start++;
  }
return 0;
}