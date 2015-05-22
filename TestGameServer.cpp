#include <string.h>
#include "gtest.h"
#include "basic.h"
#include "Card.h"
#include "Pot.h"
#include "CardBox.h"
#include "PlayerAgent.h"

using namespace std;

extern void _InitRandomSeat(unsigned int* Seats);

TEST(TestGameServer, TestRandomSeat)
{
    unsigned int OriginalSeats[MAX_PLAYER_NUM] = {0,1,2,3,4,5};
    _InitRandomSeat(OriginalSeats);
    int not_random = 0;
    for(int i=0; i < 5; i++)
    {
        if (OriginalSeats[0] == i)
        {
            not_random++;
        }
    }
    
    EXPECT_TRUE(not_random < 2);
}


extern void _CorrectActionMsg(string &action_msg, Pot* pot, CardBox* card_box, SEAT_NO seat, PlayerAgent * player ) ;

TEST(TestPot, ComplexCase2)
{
    PlayerAgent p9002;
    p9002.InitMoney(100);
    p9002.BuyJetton(100);
    
    PlayerAgent p8008;
    p8008.InitMoney(8100);
    p8008.BuyJetton(8100);

    PlayerAgent p9005;
    p9005.InitMoney(1000);
    p9005.BuyJetton(1000);

    PlayerAgent p9004;
    p9004.InitMoney(4800);
    p9004.BuyJetton(4800);
/*
        button: 9002 100 /7
        small blind: 8008 8100 /0
        big blind: 9005 1000  /1
        9004 4800 /4 
        /seat 
*/

    CardBox cardbox;
    string action_msg;

    Pot pot(50);
    pot.SetEmptySeat((SEAT_NO)2);
    pot.SetEmptySeat((SEAT_NO)3);
    pot.SetEmptySeat((SEAT_NO)5);
    pot.SetEmptySeat((SEAT_NO)6);    
            
    pot.Blind(SMALL_BLIND,50);
    p8008.Bet(50);
    pot.Blind(BIG_BLIND,100);
    p9005.Bet(100);

    ASSERT_EQ(pot.GetTotalPotAmount(), 150);

    //pot.Call((SEAT_NO)4);
    action_msg = string("call");
    _CorrectActionMsg(action_msg, &pot, &cardbox, (SEAT_NO)4, &p9004);

    ASSERT_EQ(pot.GetTotalPotAmount(), 250);

    //pot.Fold(BUTTON);
    action_msg = string("fold");
    _CorrectActionMsg(action_msg, &pot, &cardbox, BUTTON, &p9002);

    //pot.AllIn(SMALL_BLIND, 8050);
    action_msg = string("all_in");
    _CorrectActionMsg(action_msg, &pot, &cardbox, SMALL_BLIND, &p8008);
    
    //pot.AllIn(BIG_BLIND, 900);
    action_msg = string("all_in");
    _CorrectActionMsg(action_msg, &pot, &cardbox, BIG_BLIND, &p9005);

    //pot.Raise((SEAT_NO)4, 100);
    action_msg = string("raise 100");
    _CorrectActionMsg(action_msg, &pot, &cardbox, (SEAT_NO)4, &p9004);

    ASSERT_EQ(pot.GetTotalPotAmount(), 13900);
    ASSERT_EQ(true, pot.IsCurrentBetCircleFinished());
}

/*
seat/ 
button: 9003 950 
small blind: 9001 1000 
big blind: 9002 900 
3015 2150 
/seat 
blind/ 
9001: 50 
9002: 100 
/blind 
player 9001:hold/ 
HEARTS 5 
CLUBS 8 
/hold 
player 9002:hold/ 
DIAMONDS 10 
HEARTS 6 
/hold 
player 3015:hold/ 
SPADES J 
CLUBS 2 
/hold 
player 9003:hold/ 
CLUBS K 
SPADES 9 
/hold 
---pre flop round bet---
send inquire msg to player 3015 
inquire/ 
9002 800 3000 100 blind 
9001 950 2000 50 blind 
total pot: 150 
/inquire 
recv from player 3015: all_in 
send inquire msg to player 9003 
inquire/ 
3015 0 3000 2150 all_in 
9002 800 3000 100 blind 
9001 950 2000 50 blind 
total pot: 2300 
/inquire 
recv from player 9003: check 
send inquire msg to player 9001 
inquire/ 
9003 0 3000 950 all_in 
3015 0 3000 2150 all_in 
9002 800 3000 100 blind 
9001 950 2000 50 blind 
total pot: 3250 
/inquire 
recv from player 9001: fold 
send inquire msg to player 9002 
inquire/ 
9001 950 2000 50 fold 
9003 0 3000 950 all_in 
3015 0 3000 2150 all_in 
9002 800 3000 100 blind 
total pot: 3250 
/inquire 
recv from player 9002: fold 
*/

TEST(TestPot, ComplexCase3)
{
    PlayerAgent p9003;
    p9003.InitMoney(950);
    p9003.BuyJetton(950);
    
    PlayerAgent p9001;
    p9001.InitMoney(1000);
    p9001.BuyJetton(1000);

    PlayerAgent p9002;
    p9002.InitMoney(900);
    p9002.BuyJetton(900);

    PlayerAgent p3015;
    p3015.InitMoney(2150);
    p3015.BuyJetton(2150);
/*
seat/ 
button: 9003 950 
small blind: 9001 1000 
big blind: 9002 900 
3015 2150 
/seat 
*/

    CardBox cardbox;
    string action_msg;

    Pot pot(50);
    pot.SetEmptySeat((SEAT_NO)3);
    pot.SetEmptySeat((SEAT_NO)4);
    pot.SetEmptySeat((SEAT_NO)5);
    pot.SetEmptySeat((SEAT_NO)6);    
            
    pot.Blind(SMALL_BLIND,50);
    p9001.Bet(50);
    pot.Blind(BIG_BLIND,100);
    p9002.Bet(100);

    ASSERT_EQ(pot.GetTotalPotAmount(), 150);

    //pot.Call((SEAT_NO)2);
    action_msg = string("all_in");
    _CorrectActionMsg(action_msg, &pot, &cardbox, (SEAT_NO)2, &p3015);

    action_msg = string("all_in");
    _CorrectActionMsg(action_msg, &pot, &cardbox, BUTTON, &p9003);

    action_msg = string("fold");
    _CorrectActionMsg(action_msg, &pot, &cardbox, SMALL_BLIND, &p9001);

    action_msg = string("fold");
    _CorrectActionMsg(action_msg, &pot, &cardbox, BIG_BLIND, &p9002);


    ASSERT_EQ(pot.GetTotalPotAmount(), 3250);
    ASSERT_EQ(true, pot.IsCurrentBetCircleFinished());

    ASSERT_EQ(2, pot.NumOfNotFoldPlayer());

 
}



TEST(TestPot, TestUnlimitedRaise)
{
    PlayerAgent p9002;
    p9002.InitMoney(100);
    p9002.BuyJetton(100);
    
    PlayerAgent p8008;
    p8008.InitMoney(8100);
    p8008.BuyJetton(8100);

    PlayerAgent p9005;
    p9005.InitMoney(1000);
    p9005.BuyJetton(1000);

    PlayerAgent p9004;
    p9004.InitMoney(4800);
    p9004.BuyJetton(4800);
/*
        button: 9002 100 /7
        small blind: 8008 8100 /0
        big blind: 9005 1000  /1
        9004 4800 /4 
        /seat 
*/

    CardBox cardbox;
    string action_msg;

    Pot pot(50);
    pot.SetEmptySeat((SEAT_NO)2);
    pot.SetEmptySeat((SEAT_NO)3);
    pot.SetEmptySeat((SEAT_NO)5);
    pot.SetEmptySeat((SEAT_NO)6);    
            
    pot.Blind(SMALL_BLIND,50);
    p8008.Bet(50);
    pot.Blind(BIG_BLIND,100);
    p9005.Bet(100);

    ASSERT_EQ(pot.GetTotalPotAmount(), 150);

    //pot.Call((SEAT_NO)4);
    action_msg = string("raise 1000");
    _CorrectActionMsg(action_msg, &pot, &cardbox, (SEAT_NO)4, &p9004);

//     ASSERT_EQ(pot.GetTotalPotAmount(), 250);
// 
//     //pot.Fold(BUTTON);
//     action_msg = string("fold");
//     _CorrectActionMsg(action_msg, &pot, &cardbox, BUTTON, &p9002);
// 
//     //pot.AllIn(SMALL_BLIND, 8050);
//     action_msg = string("all_in");
//     _CorrectActionMsg(action_msg, &pot, &cardbox, SMALL_BLIND, &p8008);
//     
//     //pot.AllIn(BIG_BLIND, 900);
//     action_msg = string("all_in");
//     _CorrectActionMsg(action_msg, &pot, &cardbox, BIG_BLIND, &p9005);
// 
//     //pot.Raise((SEAT_NO)4, 100);
//     action_msg = string("raise 100");
//     _CorrectActionMsg(action_msg, &pot, &cardbox, (SEAT_NO)4, &p9004);
// 
//     ASSERT_EQ(pot.GetTotalPotAmount(), 13900);
//     ASSERT_EQ(true, pot.IsCurrentBetCircleFinished());
}


/*
    button: 9001 2000 /7
    small blind: 9002 2000 /0
    big blind: 9003 2000  /1
    9004 2000 /2
    9005 2000 /3
    9006 2000 /4
    /seat 
*/

TEST(TestGameserver, TestPreflopRaise)
{
    PlayerAgent p9001;
    p9001.InitMoney(2000);
    p9001.BuyJetton(2000);
    
    PlayerAgent p9002;
    p9002.InitMoney(2000);
    p9002.BuyJetton(2000);

    PlayerAgent p9003;
    p9003.InitMoney(2000);
    p9003.BuyJetton(2000);

    PlayerAgent p9004;
    p9004.InitMoney(2000);
    p9004.BuyJetton(2000);

    PlayerAgent p9005;
    p9005.InitMoney(2000);
    p9005.BuyJetton(2000);

    PlayerAgent p9006;
    p9006.InitMoney(2000);
    p9006.BuyJetton(2000);


    CardBox cardbox;
    string action_msg;

    Pot pot(50);
    pot.SetEmptySeat((SEAT_NO)5);
    pot.SetEmptySeat((SEAT_NO)6);    
            
    pot.Blind(SMALL_BLIND,50);
    p9002.Bet(50);
    pot.Blind(BIG_BLIND,100);
    p9003.Bet(100);

    ASSERT_EQ(pot.GetTotalPotAmount(), 150);
    
    action_msg = string("fold");
    _CorrectActionMsg(action_msg, &pot, &cardbox, (SEAT_NO)2, &p9004);


    action_msg = string("call");
    _CorrectActionMsg(action_msg, &pot, &cardbox, (SEAT_NO)3, &p9005);//应该询问

    action_msg = string("all_in");
    _CorrectActionMsg(action_msg, &pot, &cardbox, (SEAT_NO)4, &p9006);

    action_msg = string("fold");
    _CorrectActionMsg(action_msg, &pot, &cardbox, BUTTON, &p9001);

    action_msg = string("check");
    _CorrectActionMsg(action_msg, &pot, &cardbox, SMALL_BLIND, &p9002);

    action_msg = string("call");
    _CorrectActionMsg(action_msg, &pot, &cardbox, BIG_BLIND, &p9003);

    
    ASSERT_EQ(false, pot.IsCurrentBetCircleFinished());

    action_msg = string("call");
    _CorrectActionMsg(action_msg, &pot, &cardbox, (SEAT_NO)3, &p9005);//应该询问

    ASSERT_EQ(true, pot.IsCurrentBetCircleFinished());
    
}

/*
seat/ 
button: 2003 2500 2550 
small blind: 1013 6810 7500 
big blind: 3010 1069 5019 
3049 9552 5000 
/seat 
*/

TEST(TestGameserver, TestQuitAbnormal)
{
    PlayerAgent p2003;
    p2003.InitMoney(2500+2550);
    p2003.BuyJetton(2500);

    PlayerAgent p1013;
    p1013.InitMoney(6810+7500);
    p1013.BuyJetton(6810);

    PlayerAgent p3010;
    p3010.InitMoney(1069+5019);
    p3010.BuyJetton(1069);

    PlayerAgent p3049;
    p3049.InitMoney(9552+5000);
    p3049.BuyJetton(9552);

    CardBox cardbox;
    string action_msg;

    Pot pot(50);
    pot.SetEmptySeat((SEAT_NO)3);
    pot.SetEmptySeat((SEAT_NO)4);  
    pot.SetEmptySeat((SEAT_NO)5);  
    pot.SetEmptySeat((SEAT_NO)6); 

    pot.Blind(SMALL_BLIND,50);
    p1013.Bet(50);
    pot.Blind(BIG_BLIND,100);
    p3010.Bet(100);

    action_msg = string("all_in");
    _CorrectActionMsg(action_msg, &pot, &cardbox, (SEAT_NO)2, &p3049);

    action_msg = string("fold");
    _CorrectActionMsg(action_msg, &pot, &cardbox, BUTTON, &p2003);

    action_msg = string("fold");
    _CorrectActionMsg(action_msg, &pot, &cardbox, SMALL_BLIND, &p1013);

    action_msg = string("fold");
    _CorrectActionMsg(action_msg, &pot, &cardbox, BIG_BLIND, &p3010);

    ASSERT_EQ(true, pot.IsCurrentBetCircleFinished());

   
    ASSERT_EQ(pot.GetTotalPotAmount(), 9702);
    ASSERT_EQ(1, pot.NumOfNotFoldPlayer());
    pot.AllocatePot();
    

}

void _CheckRanks(PlayerAgent* players[])
{
    SEAT_NO sort_list_of_seats[MAX_PLAYER_NUM];    

    for(SEAT_NO seat = SMALL_BLIND; seat <= BUTTON; seat = (SEAT_NO)(seat + 1))
    {  
        sort_list_of_seats[seat] = seat;
    }

    //排序
    for(int i = 0; i < MAX_PLAYER_NUM-1; i = (i + 1))
    {
        if(players[sort_list_of_seats[i]] == NULL)
        {
            continue;
        }

        for(int j = i+1; j <= MAX_PLAYER_NUM-1; j = j + 1)
        {
            if(players[sort_list_of_seats[j]] == NULL)
            {
                continue;
            }
            if(players[sort_list_of_seats[i]]->GetScore()<players[sort_list_of_seats[j]]->GetScore())

            {
                SEAT_NO temp = sort_list_of_seats[i];
                sort_list_of_seats[i] = sort_list_of_seats[j];
                sort_list_of_seats[j] = temp;
            }
        }
    }


    //并列后面的名次要空缺
    int para_num = 1;

    //计算rank, 要考虑平手
    players[sort_list_of_seats[0]]->SetRank(1);
    for(int i=1; i<MAX_PLAYER_NUM; i++)
    {        
        if(players[sort_list_of_seats[i]] == NULL || players[sort_list_of_seats[i-1]] == NULL )
        {
            continue;
        }

        if(players[sort_list_of_seats[i]]->GetScore() == players[sort_list_of_seats[i-1]]->GetScore())        
        {
            players[sort_list_of_seats[i]]->SetRank(players[sort_list_of_seats[i-1]]->GetRank());
            para_num++;
        }
        else if(players[sort_list_of_seats[i]]->GetScore() < players[sort_list_of_seats[i-1]]->GetScore()) 
        {
            players[sort_list_of_seats[i]]->SetRank(players[sort_list_of_seats[i-1]]->GetRank()+para_num);
            para_num = 1;
        }
        else
        {
            DEBUG_ASSERT(0);
        }
    }
}

void SetHandNum(PlayerAgent* player, int hand_num)
{
    for(int i=0; i<hand_num; i++)
    {
        player->NewHand();
    }
}

void InitData(PlayerAgent* player, int money, int hand_num)
{
    player->InitMoney(money);
    SetHandNum(player, hand_num);
}

TEST(TestGameserver, TestCheckPlayerRank)
{
    PlayerAgent p1023;
    InitData(&p1023, 60950, 330);

    PlayerAgent p2006;
    InitData(&p2006, 100, 211);

    PlayerAgent p2010;
    InitData(&p2010, 150, 330);

    PlayerAgent p3028;
    InitData(&p3028, 0, 108);

    PlayerAgent p1043;
    InitData(&p1043, 50, 50);

    PlayerAgent p3004;
    InitData(&p3004, 150, 96);

    PlayerAgent p2011;
    InitData(&p2011, 100,83);

    PlayerAgent* parray[MAX_PLAYER_NUM];
    parray[0] = &p1023;
    parray[1] = &p2006;
    parray[2] = &p2010;
    parray[3] = &p3028;
    parray[4] = &p1043;
    parray[5] = &p3004;
    parray[6] = &p2011;
    parray[7] = 0;

    _CheckRanks(parray);

    ASSERT_EQ(1,p1023.GetRank());
    ASSERT_EQ(3,p2006.GetRank());
    ASSERT_EQ(2,p2010.GetRank());
    ASSERT_EQ(4,p3028.GetRank());
    ASSERT_EQ(7,p1043.GetRank());
    ASSERT_EQ(5,p3004.GetRank());
    ASSERT_EQ(6,p2011.GetRank());    
}