#include "gtest.h"
#include <vector>
#include "basic.h"
#include "Card.h"
#include "Dealer.h"
#include "CardBox.h"

TEST(TestCardBox, TestRuffle)
{    
    Dealer dealer;
    for(int i=0; i<100; i++)
    {        
        dealer.Ruffle();
        std::vector<Card> cards_dealed;

        while(dealer.RestCardNum() > 0)
        {
            Card next = dealer.DealCard();

            for(std::vector<Card>::iterator it = cards_dealed.begin(); it != cards_dealed.end(); it++)
            {
                EXPECT_NE(next, *it);
            }

            cards_dealed.insert(cards_dealed.begin(),next);
        }
    }
}

TEST(TestCardBox, TestNutHandsRoyalStraightFlush)
{
    CardBox card_box;
    card_box.DealPublicCard(Card(HEART, POINT_A));
    card_box.DealPublicCard(Card(HEART, POINT_K));
    card_box.DealPublicCard(Card(HEART, POINT_Q));
    card_box.DealPublicCard(Card(SPADE, POINT_10));
    card_box.DealPublicCard(Card(CLUB, POINT_Q));

    card_box.DealHoldCard(SMALL_BLIND, Card(SPADE, POINT_Q));
    card_box.DealHoldCard(SMALL_BLIND, Card(DIAMOND, POINT_Q));

    card_box.DealHoldCard(BIG_BLIND, Card(HEART, POINT_10));
    card_box.DealHoldCard(BIG_BLIND, Card(HEART, POINT_J));

    card_box.DealHoldCard((SEAT_NO)2, Card(CLUB, POINT_J));
    card_box.DealHoldCard((SEAT_NO)2, Card(DIAMOND, POINT_9));
    
    card_box.DealHoldCard((SEAT_NO)3, Card(HEART, POINT_2));
    card_box.DealHoldCard((SEAT_NO)3, Card(HEART, POINT_3));

    card_box.DealHoldCard((SEAT_NO)4, Card(CLUB, POINT_K));
    card_box.DealHoldCard((SEAT_NO)4, Card(DIAMOND, POINT_K));

    card_box.DealHoldCard(BUTTON, Card(CLUB, POINT_A));
    card_box.DealHoldCard(BUTTON, Card(SPADE, POINT_K));

    card_box.ShowDown();

    EXPECT_EQ(FOUR_OF_A_KIND, card_box.GetNutHandType(SMALL_BLIND));
    EXPECT_EQ(STRAIGHT_FLUSH, card_box.GetNutHandType(BIG_BLIND));
    EXPECT_EQ(STRAIGHT, card_box.GetNutHandType((SEAT_NO)2));
    EXPECT_EQ(FLUSH, card_box.GetNutHandType((SEAT_NO)3));
    EXPECT_EQ(FULL_HOUSE, card_box.GetNutHandType((SEAT_NO)4));
    EXPECT_EQ(TWO_PAIR, card_box.GetNutHandType((SEAT_NO)BUTTON));

    EXPECT_EQ(POINT_Q, card_box.GetNutHandPoint(SMALL_BLIND, 0));
    EXPECT_EQ(POINT_Q, card_box.GetNutHandPoint(SMALL_BLIND, 3));
    EXPECT_EQ(POINT_A, card_box.GetNutHandPoint(SMALL_BLIND, 4));

    EXPECT_EQ(POINT_A, card_box.GetNutHandPoint(BIG_BLIND, 0));
    EXPECT_EQ(POINT_10, card_box.GetNutHandPoint(BIG_BLIND, 4));

    EXPECT_EQ(POINT_A, card_box.GetNutHandPoint((SEAT_NO)2, 0));
    EXPECT_EQ(POINT_10, card_box.GetNutHandPoint((SEAT_NO)2, 4));

    EXPECT_EQ(POINT_A, card_box.GetNutHandPoint((SEAT_NO)3, 0));
    EXPECT_EQ(POINT_2, card_box.GetNutHandPoint((SEAT_NO)3, 4));

    EXPECT_EQ(POINT_K, card_box.GetNutHandPoint((SEAT_NO)4, 0));
    EXPECT_EQ(POINT_Q, card_box.GetNutHandPoint((SEAT_NO)4, 3));

    EXPECT_EQ(POINT_A, card_box.GetNutHandPoint(BUTTON, 0));
    EXPECT_EQ(POINT_K, card_box.GetNutHandPoint(BUTTON, 2));
    EXPECT_EQ(POINT_Q, card_box.GetNutHandPoint(BUTTON, 4));


    EXPECT_EQ(1, card_box.GetShowDownRank(BIG_BLIND));
    EXPECT_EQ(2, card_box.GetShowDownRank(SMALL_BLIND));
    EXPECT_EQ(3, card_box.GetShowDownRank((SEAT_NO)4));
    EXPECT_EQ(4, card_box.GetShowDownRank((SEAT_NO)3));
    EXPECT_EQ(5, card_box.GetShowDownRank((SEAT_NO)2));
    EXPECT_EQ(6, card_box.GetShowDownRank(BUTTON));
}

TEST(TestCardBox, TestSpecialStraightFlush)
{
    CardBox card_box;
    card_box.DealPublicCard(Card(HEART, POINT_A));
    card_box.DealPublicCard(Card(HEART, POINT_2));
    card_box.DealPublicCard(Card(HEART, POINT_3));
    card_box.DealPublicCard(Card(SPADE, POINT_10));
    card_box.DealPublicCard(Card(CLUB, POINT_Q));

    card_box.DealHoldCard(SMALL_BLIND, Card(HEART, POINT_4));
    card_box.DealHoldCard(SMALL_BLIND, Card(HEART, POINT_5));

    card_box.ShowDown();

    EXPECT_EQ(STRAIGHT_FLUSH, card_box.GetNutHandType(SMALL_BLIND));
    EXPECT_EQ(1, card_box.GetShowDownRank(SMALL_BLIND));

    EXPECT_EQ(POINT_5, card_box.GetNutHandPoint(SMALL_BLIND, 0));
    EXPECT_EQ(POINT_4, card_box.GetNutHandPoint(SMALL_BLIND, 1));
    EXPECT_EQ(POINT_3, card_box.GetNutHandPoint(SMALL_BLIND, 2));
    EXPECT_EQ(POINT_2, card_box.GetNutHandPoint(SMALL_BLIND, 3));
    EXPECT_EQ(POINT_A, card_box.GetNutHandPoint(SMALL_BLIND, 4));
}

TEST(TestCardBox, TestFullHouse)
{    
    CardBox card_box;
    card_box.DealPublicCard(Card(SPADE, POINT_8));
    card_box.DealPublicCard(Card(CLUB, POINT_8));
    card_box.DealPublicCard(Card(DIAMOND, POINT_7));
    card_box.DealPublicCard(Card(SPADE, POINT_7));
    card_box.DealPublicCard(Card(CLUB, POINT_A));

    card_box.DealHoldCard(SMALL_BLIND, Card(HEART, POINT_8));
    card_box.DealHoldCard(SMALL_BLIND, Card(HEART, POINT_A));

    card_box.DealHoldCard(BIG_BLIND, Card(DIAMOND, POINT_8));
    card_box.DealHoldCard(BIG_BLIND, Card(HEART, POINT_7));

    card_box.DealHoldCard(BUTTON, Card(CLUB, POINT_7));
    card_box.DealHoldCard(BUTTON, Card(DIAMOND, POINT_A));

    card_box.ShowDown();

    EXPECT_EQ(FULL_HOUSE, card_box.GetNutHandType(SMALL_BLIND));
    EXPECT_EQ(FULL_HOUSE, card_box.GetNutHandType(BIG_BLIND));
    EXPECT_EQ(FULL_HOUSE, card_box.GetNutHandType(BUTTON));

    EXPECT_EQ(1, card_box.GetShowDownRank(SMALL_BLIND));
    EXPECT_EQ(2, card_box.GetShowDownRank(BIG_BLIND));
    EXPECT_EQ(3, card_box.GetShowDownRank(BUTTON));
}

TEST(TestCardBox, TestFullHouse2)
{
    CardBox card_box;
    card_box.DealPublicCard(Card(HEART, POINT_6));
    card_box.DealPublicCard(Card(DIAMOND, POINT_Q));
    card_box.DealPublicCard(Card(DIAMOND, POINT_6));
    card_box.DealPublicCard(Card(DIAMOND, POINT_K));
    card_box.DealPublicCard(Card(SPADE, POINT_Q));

    card_box.DealHoldCard(SMALL_BLIND, Card(DIAMOND, POINT_A));
    card_box.DealHoldCard(SMALL_BLIND, Card(CLUB, POINT_Q));

    card_box.DealHoldCard(BIG_BLIND, Card(CLUB, POINT_6));
    card_box.DealHoldCard(BIG_BLIND, Card(CLUB, POINT_4));

    card_box.ShowDown();
    EXPECT_EQ(FULL_HOUSE, card_box.GetNutHandType(SMALL_BLIND));
    EXPECT_EQ(FULL_HOUSE, card_box.GetNutHandType(BIG_BLIND));    

    EXPECT_EQ(1, card_box.GetShowDownRank(SMALL_BLIND));
    EXPECT_EQ(2, card_box.GetShowDownRank(BIG_BLIND));
}

TEST(TestCardBox, TestPairs)
{
    CardBox card_box;
    card_box.DealPublicCard(Card(SPADE, POINT_9));
    card_box.DealPublicCard(Card(CLUB, POINT_8));
    card_box.DealPublicCard(Card(DIAMOND, POINT_7));
    card_box.DealPublicCard(Card(SPADE, POINT_6));
    card_box.DealPublicCard(Card(CLUB, POINT_A));

    card_box.DealHoldCard(SMALL_BLIND, Card(HEART, POINT_A));
    card_box.DealHoldCard(SMALL_BLIND, Card(HEART, POINT_J));

    card_box.DealHoldCard(BIG_BLIND, Card(HEART, POINT_8));
    card_box.DealHoldCard(BIG_BLIND, Card(HEART, POINT_7));

    card_box.DealHoldCard(SEAT_NO(2), Card(SPADE, POINT_8));
    card_box.DealHoldCard(SEAT_NO(2), Card(HEART, POINT_9));

    card_box.DealHoldCard(SEAT_NO(3), Card(DIAMOND, POINT_8));
    card_box.DealHoldCard(SEAT_NO(3), Card(CLUB, POINT_9));

    card_box.ShowDown();

    EXPECT_EQ(ONE_PAIR, card_box.GetNutHandType(SMALL_BLIND));
    EXPECT_EQ(TWO_PAIR, card_box.GetNutHandType(BIG_BLIND));
    EXPECT_EQ(TWO_PAIR, card_box.GetNutHandType(SEAT_NO(2)));
    EXPECT_EQ(TWO_PAIR, card_box.GetNutHandType(SEAT_NO(3)));

    EXPECT_EQ(POINT_A, card_box.GetNutHandPoint(SMALL_BLIND, 0));
    EXPECT_EQ(POINT_A, card_box.GetNutHandPoint(SMALL_BLIND, 1));
    EXPECT_EQ(POINT_J, card_box.GetNutHandPoint(SMALL_BLIND, 2));
    EXPECT_EQ(POINT_9, card_box.GetNutHandPoint(SMALL_BLIND, 3));
    EXPECT_EQ(POINT_8, card_box.GetNutHandPoint(SMALL_BLIND, 4));

    EXPECT_EQ(1, card_box.GetShowDownRank(SEAT_NO(3)));
    EXPECT_EQ(1, card_box.GetShowDownRank(SEAT_NO(2)));
    EXPECT_EQ(2, card_box.GetShowDownRank(BIG_BLIND));
    EXPECT_EQ(3, card_box.GetShowDownRank(SMALL_BLIND));
}

TEST(TestCardBox, TestStraights)
{
    CardBox card_box;
    card_box.DealPublicCard(Card(SPADE, POINT_5));
    card_box.DealPublicCard(Card(CLUB, POINT_4));
    card_box.DealPublicCard(Card(DIAMOND, POINT_3));
    card_box.DealPublicCard(Card(SPADE, POINT_K));
    card_box.DealPublicCard(Card(CLUB, POINT_A));

    card_box.DealHoldCard(SMALL_BLIND, Card(HEART, POINT_6));
    card_box.DealHoldCard(SMALL_BLIND, Card(HEART, POINT_7));

    card_box.DealHoldCard(BIG_BLIND, Card(CLUB, POINT_6));
    card_box.DealHoldCard(BIG_BLIND, Card(HEART, POINT_2));

    card_box.DealHoldCard(SEAT_NO(2), Card(CLUB, POINT_7));
    card_box.DealHoldCard(SEAT_NO(2), Card(DIAMOND, POINT_2));

    card_box.ShowDown();

    EXPECT_EQ(STRAIGHT, card_box.GetNutHandType(SMALL_BLIND));
    EXPECT_EQ(STRAIGHT, card_box.GetNutHandType(BIG_BLIND));
    EXPECT_EQ(STRAIGHT, card_box.GetNutHandType(SEAT_NO(2)));

    EXPECT_EQ(POINT_7, card_box.GetNutHandPoint(SMALL_BLIND, 0));
    EXPECT_EQ(POINT_3, card_box.GetNutHandPoint(SMALL_BLIND, 4));

    EXPECT_EQ(POINT_6, card_box.GetNutHandPoint(BIG_BLIND, 0));
    EXPECT_EQ(POINT_2, card_box.GetNutHandPoint(BIG_BLIND, 4));

    EXPECT_EQ(POINT_5, card_box.GetNutHandPoint(SEAT_NO(2), 0));
    EXPECT_EQ(POINT_A, card_box.GetNutHandPoint(SEAT_NO(2), 4));

    EXPECT_EQ(1, card_box.GetShowDownRank(SMALL_BLIND));
    EXPECT_EQ(2, card_box.GetShowDownRank(BIG_BLIND));
    EXPECT_EQ(3, card_box.GetShowDownRank(SEAT_NO(2)));
}

TEST(TestCardBox, TestHighCards)
{
    CardBox card_box;
    card_box.DealPublicCard(Card(SPADE, POINT_8));
    card_box.DealPublicCard(Card(CLUB, POINT_9));
    card_box.DealPublicCard(Card(DIAMOND, POINT_3));
    card_box.DealPublicCard(Card(SPADE, POINT_K));
    card_box.DealPublicCard(Card(CLUB, POINT_J));

    card_box.DealHoldCard(SMALL_BLIND, Card(HEART, POINT_6));
    card_box.DealHoldCard(SMALL_BLIND, Card(HEART, POINT_7));

    card_box.DealHoldCard(BIG_BLIND, Card(CLUB, POINT_A));
    card_box.DealHoldCard(BIG_BLIND, Card(HEART, POINT_2));

    card_box.DealHoldCard(SEAT_NO(2), Card(CLUB, POINT_Q));
    card_box.DealHoldCard(SEAT_NO(2), Card(DIAMOND, POINT_2));

    card_box.DealHoldCard(SEAT_NO(3), Card(SPADE, POINT_Q));
    card_box.DealHoldCard(SEAT_NO(3), Card(DIAMOND, POINT_4));

    card_box.ShowDown();

    EXPECT_EQ(HIGH_CARD, card_box.GetNutHandType(SMALL_BLIND));
    EXPECT_EQ(HIGH_CARD, card_box.GetNutHandType(BIG_BLIND));
    EXPECT_EQ(HIGH_CARD, card_box.GetNutHandType(SEAT_NO(2)));
    EXPECT_EQ(HIGH_CARD, card_box.GetNutHandType(SEAT_NO(3)));

    EXPECT_EQ(1, card_box.GetShowDownRank(BIG_BLIND));    
    EXPECT_EQ(2, card_box.GetShowDownRank(SEAT_NO(2)));
    EXPECT_EQ(2, card_box.GetShowDownRank(SEAT_NO(3)));
    EXPECT_EQ(3, card_box.GetShowDownRank(SMALL_BLIND));
}

TEST(TestCardBox, TestThreeOfAKind)
{
    CardBox card_box;
    card_box.DealPublicCard(Card(SPADE, POINT_8));
    card_box.DealPublicCard(Card(CLUB, POINT_8));
    card_box.DealPublicCard(Card(DIAMOND, POINT_3));
    card_box.DealPublicCard(Card(SPADE, POINT_K));
    card_box.DealPublicCard(Card(CLUB, POINT_J));

    card_box.DealHoldCard(SMALL_BLIND, Card(HEART, POINT_8));
    card_box.DealHoldCard(SMALL_BLIND, Card(CLUB, POINT_9));

    card_box.DealHoldCard(BIG_BLIND, Card(DIAMOND, POINT_8));
    card_box.DealHoldCard(BIG_BLIND, Card(HEART, POINT_Q));

    card_box.DealHoldCard(SEAT_NO(2), Card(CLUB, POINT_K));
    card_box.DealHoldCard(SEAT_NO(2), Card(HEART, POINT_K));

    card_box.DealHoldCard(SEAT_NO(3), Card(SPADE, POINT_J));
    card_box.DealHoldCard(SEAT_NO(3), Card(HEART, POINT_J));

    card_box.ShowDown();

    EXPECT_EQ(THREE_OF_A_KIND, card_box.GetNutHandType(SMALL_BLIND));
    EXPECT_EQ(THREE_OF_A_KIND, card_box.GetNutHandType(BIG_BLIND));
    EXPECT_EQ(FULL_HOUSE, card_box.GetNutHandType(SEAT_NO(2)));
    EXPECT_EQ(FULL_HOUSE, card_box.GetNutHandType(SEAT_NO(3)));

    EXPECT_EQ(1, card_box.GetShowDownRank(SEAT_NO(2)));
    EXPECT_EQ(2, card_box.GetShowDownRank(SEAT_NO(3)));
    EXPECT_EQ(3, card_box.GetShowDownRank(BIG_BLIND));
    EXPECT_EQ(4, card_box.GetShowDownRank(SMALL_BLIND));
}


TEST(TestCardBox, TestRankFirstFoldThreeOfAKind)
{
    CardBox card_box;
    card_box.DealPublicCard(Card(SPADE, POINT_8));
    card_box.DealPublicCard(Card(CLUB, POINT_8));
    card_box.DealPublicCard(Card(DIAMOND, POINT_3));
    card_box.DealPublicCard(Card(SPADE, POINT_K));
    card_box.DealPublicCard(Card(CLUB, POINT_J));

    card_box.DealHoldCard(SMALL_BLIND, Card(HEART, POINT_8));
    card_box.DealHoldCard(SMALL_BLIND, Card(CLUB, POINT_9));

    card_box.DealHoldCard(BIG_BLIND, Card(DIAMOND, POINT_8));
    card_box.DealHoldCard(BIG_BLIND, Card(HEART, POINT_Q));

    card_box.DealHoldCard(SEAT_NO(2), Card(CLUB, POINT_K));
    card_box.DealHoldCard(SEAT_NO(2), Card(HEART, POINT_K));

    card_box.DealHoldCard(SEAT_NO(3), Card(SPADE, POINT_J));
    card_box.DealHoldCard(SEAT_NO(3), Card(HEART, POINT_J));

    card_box.GiveUp(SEAT_NO(2));

    card_box.ShowDown();

    EXPECT_EQ(THREE_OF_A_KIND, card_box.GetNutHandType(SMALL_BLIND));
    EXPECT_EQ(THREE_OF_A_KIND, card_box.GetNutHandType(BIG_BLIND));
    EXPECT_EQ(NONE_HAND, card_box.GetNutHandType(SEAT_NO(2)));
    EXPECT_EQ(FULL_HOUSE, card_box.GetNutHandType(SEAT_NO(3)));

    EXPECT_EQ(4, card_box.GetShowDownRank(SEAT_NO(2)));
    EXPECT_EQ(1, card_box.GetShowDownRank(SEAT_NO(3)));
    EXPECT_EQ(2, card_box.GetShowDownRank(BIG_BLIND));
    EXPECT_EQ(3, card_box.GetShowDownRank(SMALL_BLIND));
}

TEST(TestCardBox, TestPairsAndStraight)
{
    CardBox card_box;
    card_box.DealPublicCard(Card(SPADE, POINT_3));
    card_box.DealPublicCard(Card(DIAMOND , POINT_J));
    card_box.DealPublicCard(Card(SPADE , POINT_7));
    card_box.DealPublicCard(Card(CLUB , POINT_5));
    card_box.DealPublicCard(Card(CLUB, POINT_6));

    card_box.DealHoldCard(SMALL_BLIND, Card(SPADE, POINT_4));
    card_box.DealHoldCard(SMALL_BLIND, Card(CLUB, POINT_2));

    card_box.DealHoldCard(BIG_BLIND, Card(DIAMOND, POINT_10));
    card_box.DealHoldCard(BIG_BLIND, Card(DIAMOND, POINT_7));

    card_box.DealHoldCard(SEAT_NO(2), Card(HEART , POINT_Q));
    card_box.DealHoldCard(SEAT_NO(2), Card(CLUB, POINT_Q));

    card_box.ShowDown();

    EXPECT_EQ(STRAIGHT, card_box.GetNutHandType(SMALL_BLIND));
    EXPECT_EQ(ONE_PAIR, card_box.GetNutHandType(BIG_BLIND));
    EXPECT_EQ(ONE_PAIR, card_box.GetNutHandType(SEAT_NO(2)));

    EXPECT_EQ(1, card_box.GetShowDownRank(SMALL_BLIND));
    EXPECT_EQ(2, card_box.GetShowDownRank(SEAT_NO(2)));
    EXPECT_EQ(3, card_box.GetShowDownRank(BIG_BLIND));
    
}


TEST(TestCardBox, TestPairAfterFour)
{
    CardBox card_box;
    card_box.DealPublicCard(Card(SPADE, POINT_4));
    card_box.DealPublicCard(Card(SPADE , POINT_K));
    card_box.DealPublicCard(Card(HEART , POINT_4));
    card_box.DealPublicCard(Card(DIAMOND , POINT_K));
    card_box.DealPublicCard(Card(SPADE, POINT_2));

    //9001
    card_box.DealHoldCard(SMALL_BLIND, Card(CLUB, POINT_K));
    card_box.DealHoldCard(SMALL_BLIND, Card(HEART, POINT_K));

    //9002
    card_box.DealHoldCard(BIG_BLIND, Card(SPADE, POINT_Q));
    card_box.DealHoldCard(BIG_BLIND, Card(HEART, POINT_10));

    //9003
    card_box.DealHoldCard(SEAT_NO(2), Card(CLUB, POINT_2));
    card_box.DealHoldCard(SEAT_NO(2), Card(DIAMOND, POINT_8));

    //9004
    card_box.DealHoldCard(SEAT_NO(3), Card(SPADE, POINT_10));
    card_box.DealHoldCard(SEAT_NO(3), Card(SPADE, POINT_J));

    //9005
    card_box.DealHoldCard(SEAT_NO(4), Card(CLUB, POINT_8));
    card_box.DealHoldCard(SEAT_NO(4), Card(CLUB, POINT_4));

    card_box.ShowDown();

    EXPECT_EQ(FOUR_OF_A_KIND, card_box.GetNutHandType(SMALL_BLIND));
    EXPECT_EQ(TWO_PAIR, card_box.GetNutHandType(BIG_BLIND));
    EXPECT_EQ(TWO_PAIR, card_box.GetNutHandType(SEAT_NO(2)));
    EXPECT_EQ(FLUSH, card_box.GetNutHandType(SEAT_NO(3)));
    EXPECT_EQ(FULL_HOUSE, card_box.GetNutHandType(SEAT_NO(4)));

    //EXPECT_EQ(1, card_box.GetShowDownRank(SMALL_BLIND));
    //EXPECT_EQ(2, card_box.GetShowDownRank(SEAT_NO(2)));
    //EXPECT_EQ(3, card_box.GetShowDownRank(BIG_BLIND));

}

