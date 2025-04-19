#include "scanner.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace RoomScanner
{

class ScannerTest : public ::testing::Test
{
    void SetUp() override{}
    void TearDown() override{}
};

TEST_F(ScannerTest, Constructor)
{
    ASSERT_TRUE(true);
}

}