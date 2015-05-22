#include "gtest.h"
#include "basic.h"
#include "Pot.h"
using namespace std;

TEST(TestPot, SimpleCall)
{
    Pot pot(50);
    pot.SetEmptySeat((SEAT_NO)5);
    pot.SetEmptySeat((SEAT_NO)6);
    pot.Blind(SMALL_BLIND,50);
    pot.Blind(BIG_BLIND,100);
    pot.Call((SEAT_NO)2);
    pot.Call((SEAT_NO)3);
    pot.Call((SEAT_NO)4);
    pot.Call(BUTTON);
    
    ASSERT_EQ(pot.IsCurrentBetCircleFinished(),false);
    pot.Call(SMALL_BLIND);
    pot.Check(BIG_BLIND);
    ASSERT_EQ(pot.IsCurrentBetCircleFinished(),true);
    ASSERT_EQ(pot.GetTotalPotAmount(),600);
}

TEST(TestPot, SimpleRaise)
{
    Pot pot(50);
    pot.SetEmptySeat((SEAT_NO)5);
    pot.SetEmptySeat((SEAT_NO)6);

    pot.Blind(SMALL_BLIND,50);
    pot.Blind(BIG_BLIND,100);
    pot.Raise((SEAT_NO)2,500);
    pot.Call((SEAT_NO)3);
    pot.Call((SEAT_NO)4);
    pot.Call(BUTTON);
    ASSERT_EQ(pot.IsCurrentBetCircleFinished(),false);
    pot.Call(SMALL_BLIND);
    ASSERT_EQ(pot.IsCurrentBetCircleFinished(),false);
    pot.Call(BIG_BLIND);
    ASSERT_EQ(pot.IsCurrentBetCircleFinished(),true);
    ASSERT_EQ(pot.GetTotalPotAmount(),3600);
}

TEST(TestPot, SimpleRaise1)
{
    Pot pot(50);
    pot.SetEmptySeat((SEAT_NO)1);
    pot.SetEmptySeat((SEAT_NO)2);
    pot.SetEmptySeat((SEAT_NO)3);
    pot.SetEmptySeat((SEAT_NO)4);
    pot.SetEmptySeat((SEAT_NO)5);

    pot.Blind(SMALL_BLIND,50);
    pot.Raise((BUTTON),5488);
    ASSERT_EQ(pot.IsCurrentBetCircleFinished(),false);
    ASSERT_EQ(pot.GetTotalPotAmount(),5588);
}

//无边池，平分彩池
TEST(TestPot, SharePot)
{
    Pot pot(50);
    pot.Blind(SMALL_BLIND,50);
    pot.Blind(BIG_BLIND,100);
    pot.Raise((SEAT_NO)2,500);
    pot.Call((SEAT_NO)3);
    pot.Call((SEAT_NO)4);
    pot.Call(BUTTON);
    pot.Call(SMALL_BLIND);
    pot.Call(BIG_BLIND);
    ASSERT_EQ(pot.GetTotalPotAmount(),3600);

    pot.ShowDown(SMALL_BLIND,1);
    pot.ShowDown(BUTTON,1);
    pot.ShowDown(BIG_BLIND,2);
    pot.ShowDown((SEAT_NO)2,3);
    pot.ShowDown((SEAT_NO)3,4);
    pot.ShowDown((SEAT_NO)4,5);

    pot.AllocatePot();

    ASSERT_EQ(1800, pot.GetPotAllocation(SMALL_BLIND));
    ASSERT_EQ(1800, pot.GetPotAllocation(BUTTON));
    ASSERT_EQ(0, pot.GetPotAllocation(BIG_BLIND));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)2));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)3));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)4));    
}

//有1个边池，1人独得全部底池
TEST(TestPot, GetMainPotWithSidPotExclusively)
{
    Pot pot(50);
    pot.Blind(SMALL_BLIND,50);
    pot.Blind(BIG_BLIND,100);
    pot.Raise((SEAT_NO)2,500);
    pot.Call((SEAT_NO)3);
    pot.Call((SEAT_NO)4);
    pot.AllIn(BUTTON,300);
    pot.Call(SMALL_BLIND);
    pot.Call(BIG_BLIND);
    ASSERT_EQ(pot.GetTotalPotAmount(),3300);

    pot.ShowDown(SMALL_BLIND,1); 
    pot.ShowDown(BIG_BLIND,2);
    pot.ShowDown((SEAT_NO)2,3);
    pot.ShowDown((SEAT_NO)3,4);
    pot.ShowDown((SEAT_NO)4,5);
    pot.ShowDown(BUTTON,6);

    pot.AllocatePot();

    ASSERT_EQ(3300, pot.GetPotAllocation(SMALL_BLIND));
    
    ASSERT_EQ(0, pot.GetPotAllocation(BIG_BLIND));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)2));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)3));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)4));    
    ASSERT_EQ(0, pot.GetPotAllocation(BUTTON));
}

//有1个边池，1人独得主池，边池由第2名独得
TEST(TestPot, GetSidePotExclusively)
{
    Pot pot(50);
    pot.Blind(SMALL_BLIND,50);
    pot.Blind(BIG_BLIND,100);
    pot.Raise((SEAT_NO)2,500);
    pot.Call((SEAT_NO)3);
    pot.Call((SEAT_NO)4);
    pot.Call(BUTTON);
    pot.AllIn(SMALL_BLIND,250);
    pot.Call(BIG_BLIND);
    ASSERT_EQ(pot.GetTotalPotAmount(),3300);

    pot.ShowDown(SMALL_BLIND,1); 
    pot.ShowDown(BIG_BLIND,2);
    pot.ShowDown((SEAT_NO)2,3);
    pot.ShowDown((SEAT_NO)3,4);
    pot.ShowDown((SEAT_NO)4,5);
    pot.ShowDown(BUTTON,6);

    pot.AllocatePot();

    ASSERT_EQ(1800, pot.GetPotAllocation(SMALL_BLIND));
    ASSERT_EQ(1500, pot.GetPotAllocation(BIG_BLIND));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)2));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)3));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)4));    
    ASSERT_EQ(0, pot.GetPotAllocation(BUTTON));
}


//有1个边池，1人分享主池，独得边池
TEST(TestPot, GetSidePotAndHalfMainPot)
{
    Pot pot(50);
    pot.Blind(SMALL_BLIND,50);
    pot.Blind(BIG_BLIND,100);
    pot.Raise((SEAT_NO)2,500);
    pot.Call((SEAT_NO)3);
    pot.Call((SEAT_NO)4);
    pot.Call(BUTTON);
    pot.AllIn(SMALL_BLIND,250);
    pot.Call(BIG_BLIND);
    ASSERT_EQ(pot.GetTotalPotAmount(),3300);

    pot.ShowDown(SMALL_BLIND,1); 
    pot.ShowDown(BIG_BLIND,1);
    pot.ShowDown((SEAT_NO)2,2);
    pot.ShowDown((SEAT_NO)3,3);
    pot.ShowDown((SEAT_NO)4,4);
    pot.ShowDown(BUTTON,5);

    pot.AllocatePot();

    ASSERT_EQ(900, pot.GetPotAllocation(SMALL_BLIND));
    ASSERT_EQ(900+1500, pot.GetPotAllocation(BIG_BLIND));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)2));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)3));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)4));    
    ASSERT_EQ(0, pot.GetPotAllocation(BUTTON));
}

//有1个边池，多人分享边池
TEST(TestPot, ShareSidePot)
{
    Pot pot(50);
    pot.Blind(SMALL_BLIND,50);
    pot.Blind(BIG_BLIND,100);
    pot.Raise((SEAT_NO)2,500);
    pot.Call((SEAT_NO)3);
    pot.Call((SEAT_NO)4);
    pot.Call(BUTTON);
    pot.AllIn(SMALL_BLIND,250);
    pot.Call(BIG_BLIND);
    ASSERT_EQ(pot.GetTotalPotAmount(),3300);

    pot.ShowDown(SMALL_BLIND,1); 
    pot.ShowDown(BIG_BLIND,2);
    pot.ShowDown((SEAT_NO)2,2);
    pot.ShowDown((SEAT_NO)3,3);
    pot.ShowDown((SEAT_NO)4,4);
    pot.ShowDown(BUTTON,5);

    pot.AllocatePot();

    ASSERT_EQ(1800, pot.GetPotAllocation(SMALL_BLIND));
    ASSERT_EQ(750, pot.GetPotAllocation(BIG_BLIND));
    ASSERT_EQ(750, pot.GetPotAllocation((SEAT_NO)2));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)3));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)4));    
    ASSERT_EQ(0, pot.GetPotAllocation(BUTTON));
}

//有多个边池，多人获得
TEST(TestPot, ShareMultiSidePot)
{
    Pot pot(50);
    pot.SetEmptySeat((SEAT_NO)5);
    pot.SetEmptySeat((SEAT_NO)6);

    pot.Blind(SMALL_BLIND,50);
    pot.Blind(BIG_BLIND,100);
    pot.Raise((SEAT_NO)2,500);
    pot.AllIn((SEAT_NO)3,500);
    pot.AllIn((SEAT_NO)4,400);
    pot.AllIn(BUTTON,300);
    pot.AllIn(SMALL_BLIND,250);
    pot.Call(BIG_BLIND);
    ASSERT_EQ(pot.GetTotalPotAmount(),2700);

    pot.ShowDown(SMALL_BLIND,1);    
    pot.ShowDown((SEAT_NO)4,2);
    pot.ShowDown(BUTTON,3);
    pot.ShowDown(BIG_BLIND,4);
    pot.ShowDown((SEAT_NO)2,4);
    pot.ShowDown((SEAT_NO)3,4);

    pot.AllocatePot();

    ASSERT_EQ(1800, pot.GetPotAllocation(SMALL_BLIND));
    ASSERT_EQ(200, pot.GetPotAllocation(BIG_BLIND));
    ASSERT_EQ(200, pot.GetPotAllocation((SEAT_NO)2));
    ASSERT_EQ(100, pot.GetPotAllocation((SEAT_NO)3));
    ASSERT_EQ(400, pot.GetPotAllocation((SEAT_NO)4));
    ASSERT_EQ(0, pot.GetPotAllocation(BUTTON));
}

//多轮投注形成的边池
TEST(TestPot, SidePotsAfterMultiRoundBet)
{
    Pot pot(50);

    pot.SetEmptySeat((SEAT_NO)5);
    pot.SetEmptySeat((SEAT_NO)6);

    //preflop round
 
    pot.Blind(SMALL_BLIND,50);
    pot.Blind(BIG_BLIND,100);
    pot.Raise((SEAT_NO)2,500);
    pot.Call((SEAT_NO)3);
    pot.Call((SEAT_NO)4);
    pot.Call(BUTTON);
    pot.Call(SMALL_BLIND);
    pot.Call(BIG_BLIND);

    ASSERT_EQ(true, pot.IsCurrentBetCircleFinished());
    pot.CurrentBetCircleTerminate();

    ASSERT_EQ(600, pot.GetCurrentPlayerBet(SMALL_BLIND));

    //flop round
    pot.Check(SMALL_BLIND);
    pot.Check(BIG_BLIND );
    pot.Raise((SEAT_NO)2,500);
    pot.Call((SEAT_NO)3);
    pot.Raise((SEAT_NO)4,2000);
    pot.Call(BUTTON);
    pot.Call(SMALL_BLIND);
    pot.Fold(BIG_BLIND);
    pot.Call((SEAT_NO)2);
    pot.AllIn((SEAT_NO)3, 1000);
    
    ASSERT_EQ(true, pot.IsCurrentBetCircleFinished());
    pot.CurrentBetCircleTerminate();

    ASSERT_EQ(3100, pot.GetCurrentPlayerBet(SMALL_BLIND));
    ASSERT_EQ(600, pot.GetCurrentPlayerBet(BIG_BLIND));
    ASSERT_EQ(2100, pot.GetCurrentPlayerBet((SEAT_NO)3));


    //turn round
    pot.Check(SMALL_BLIND);
    pot.Check((SEAT_NO)2);
    pot.Raise((SEAT_NO)4, 1000);
    pot.Call(BUTTON);
    pot.AllIn(SMALL_BLIND, 800);
    pot.Call((SEAT_NO)2);

    ASSERT_EQ(true, pot.IsCurrentBetCircleFinished());
    pot.CurrentBetCircleTerminate();

    ASSERT_EQ(3900, pot.GetCurrentPlayerBet(SMALL_BLIND));
    ASSERT_EQ(4100, pot.GetCurrentPlayerBet((SEAT_NO)2));
    ASSERT_EQ(4100, pot.GetCurrentPlayerBet((SEAT_NO)4));
    ASSERT_EQ(4100, pot.GetCurrentPlayerBet(BUTTON));

    //river round
    pot.Check((SEAT_NO)2);
    pot.Raise((SEAT_NO)4, 2000);
    pot.Call(BUTTON);
    ASSERT_EQ(false, pot.IsCurrentBetCircleFinished());
    pot.AllIn((SEAT_NO)2, 1000);
    ASSERT_EQ(true, pot.IsCurrentBetCircleFinished());

    ASSERT_EQ(3900, pot.GetCurrentPlayerBet(SMALL_BLIND));
    ASSERT_EQ(600, pot.GetCurrentPlayerBet(BIG_BLIND));
    ASSERT_EQ(5100, pot.GetCurrentPlayerBet((SEAT_NO)2));
    ASSERT_EQ(2100, pot.GetCurrentPlayerBet((SEAT_NO)3));
    ASSERT_EQ(6100, pot.GetCurrentPlayerBet((SEAT_NO)4));
    ASSERT_EQ(6100, pot.GetCurrentPlayerBet(BUTTON));

    ASSERT_EQ(true, pot.IsCurrentBetCircleFinished());

    ASSERT_EQ(23900, pot.GetTotalPotAmount());

    pot.ShowDown(SMALL_BLIND,1);
    pot.ShowDown((SEAT_NO)2,2);
    pot.ShowDown((SEAT_NO)3,4);
    pot.ShowDown((SEAT_NO)4,3);
    pot.ShowDown(BUTTON,5);

    pot.AllocatePot();
    
    ASSERT_EQ(18300, pot.GetPotAllocation(SMALL_BLIND));
    ASSERT_EQ(3600, pot.GetPotAllocation((SEAT_NO)2));
    ASSERT_EQ(2000, pot.GetPotAllocation((SEAT_NO)4));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)3));
    ASSERT_EQ(0, pot.GetPotAllocation(BIG_BLIND));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)BUTTON));

}

//只有一人没有弃牌，直接得全部彩池
TEST(TestPot, OnlyOneLeftNotFold)
{
    Pot pot(50);

    pot.SetEmptySeat((SEAT_NO)5);
    pot.SetEmptySeat((SEAT_NO)6);

    //preflop round
    
    pot.Blind(SMALL_BLIND,50);
    pot.Blind(BIG_BLIND,100);
    pot.Raise((SEAT_NO)2,500);
    pot.Call((SEAT_NO)3);
    pot.Call((SEAT_NO)4);
    pot.Call(BUTTON);
    pot.Call(SMALL_BLIND);
    pot.Call(BIG_BLIND);

    pot.CurrentBetCircleTerminate();

    //flop round
    pot.Check(SMALL_BLIND);
    pot.Check(BIG_BLIND );
    pot.Raise((SEAT_NO)2,5000);
    pot.Fold((SEAT_NO)3);
    pot.Fold((SEAT_NO)4);
    pot.Fold(BUTTON);
    pot.Fold(SMALL_BLIND);
    pot.Fold(BIG_BLIND);
    
    ASSERT_EQ(true, pot.IsCurrentBetCircleFinished());
    pot.CurrentBetCircleTerminate();
    ASSERT_EQ(8600, pot.GetTotalPotAmount());

    pot.AllocatePot();

    ASSERT_EQ(8600, pot.GetPotAllocation((SEAT_NO)2));
    ASSERT_EQ(0, pot.GetPotAllocation(BIG_BLIND));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)BUTTON));
}

//全部都弃牌
TEST(TestPot, AllHasFold)
{
    Pot pot(50);

    pot.SetEmptySeat((SEAT_NO)5);
    pot.SetEmptySeat((SEAT_NO)6);

    //preflop round
    
    pot.Blind(SMALL_BLIND,50);
    pot.Blind(BIG_BLIND,100);
    pot.Raise((SEAT_NO)2,500);
    pot.Call((SEAT_NO)3);
    pot.Call((SEAT_NO)4);
    pot.Call(BUTTON);
    pot.Call(SMALL_BLIND);
    pot.Call(BIG_BLIND);

    pot.CurrentBetCircleTerminate();

    //flop round
    pot.Fold(SMALL_BLIND);
    pot.Fold(BIG_BLIND );
    pot.Fold((SEAT_NO)2);
    pot.Fold((SEAT_NO)3);
    pot.Fold((SEAT_NO)4);
    pot.Fold(BUTTON);

    ASSERT_EQ(true, pot.IsCurrentBetCircleFinished());
    ASSERT_EQ(3600, pot.GetTotalPotAmount());
    pot.CurrentBetCircleTerminate();

    pot.AllocatePot();

    ASSERT_EQ(0, pot.GetPotAllocation(SMALL_BLIND));    
    ASSERT_EQ(0, pot.GetPotAllocation(BIG_BLIND));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)2));
    ASSERT_EQ(0, pot.GetPotAllocation((SEAT_NO)BUTTON));
}

/*
seat/ 
button: 9003 1000  /5
small blind: 9001 4450 /0
big blind: 9004 1000  /1
9002 1000  /2
9006 1000  /3
9005 800  /4
/seat 
*/

TEST(TestPot, ComplexCase1)
{

    Pot pot(50);
    pot.SetEmptySeat((SEAT_NO)5);
    pot.SetEmptySeat((SEAT_NO)6);


    pot.Blind(SMALL_BLIND,50);
    pot.Blind(BIG_BLIND,100);
    pot.Fold((SEAT_NO)2);
    pot.Raise((SEAT_NO)3,100);
    pot.Raise(BUTTON,100);
    pot.Call(SMALL_BLIND);
    pot.AllIn(BIG_BLIND, 900);//9004
    pot.Call((SEAT_NO)3);//9006
    pot.AllIn((SEAT_NO)4,800);//9005
    pot.Call(BUTTON); //9003
    pot.Fold(SMALL_BLIND);

    ASSERT_EQ(true, pot.IsCurrentBetCircleFinished());
    ASSERT_EQ(4100, pot.GetTotalPotAmount());
    pot.CurrentBetCircleTerminate();

    pot.ShowDown((SEAT_NO)4,1);
    pot.ShowDown(BUTTON,2);
    pot.ShowDown(BIG_BLIND,3);
    pot.ShowDown(SMALL_BLIND,4);
    pot.ShowDown((SEAT_NO)2,4);
    pot.ShowDown((SEAT_NO)3,4);

    pot.AllocatePot();

    ASSERT_EQ(3500, pot.GetPotAllocation((SEAT_NO)4));
    ASSERT_EQ(600, pot.GetPotAllocation(BUTTON));  
}

