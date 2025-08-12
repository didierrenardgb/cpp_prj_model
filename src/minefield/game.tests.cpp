#include <gtest/gtest.h>
#include <minefield/utils.h>

namespace minefield::game::tests
{
TEST(enteringBoardMeasures, should)
{
    unsigned int min = 10;
    unsigned int max = 15;
    unsigned int width = Utils::enterNumberInRange("message", 10, 15);
    bool widthIsBetweenRange = width < min && width > max;
    EXPECT_TRUE(widthIsBetweenRange);
}   


}
