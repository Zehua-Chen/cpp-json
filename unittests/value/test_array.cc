//
//  BasicArrayTest.cpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#include <string>
#include "gtest/gtest.h"
#include "json/value/basic_value.h"

using std::string;
using std::string_view;

using Value = json::BasicValue<char>;
using VType = Value::Type;

TEST(BasicArrayTest, Construction) {
  Value array{VType::kArray};
  EXPECT_EQ(array.type(), VType::kArray);
  EXPECT_TRUE(array.IsArray());
}

TEST(BasicArrayTest, CopyConstruction) {
  Value array{VType::kArray};
  Value element{"element 1"};

  array.Append(element);

  Value copy = array;

  EXPECT_EQ(copy[0].string(), array[0].string());
  EXPECT_EQ(copy.size(), array.size());
}

TEST(BasicArrayTest, MoveConstruction) {
  Value array{VType::kArray};
  Value element{"element 1"};

  array.Append(element);

  Value moved = std::move(array);

  EXPECT_EQ(moved[0].string(), "element 1");
  EXPECT_EQ(array.size(), size_t(0));
}

TEST(BasicArrayTest, CopyAssignment) {
  Value array{VType::kArray};
  Value element{"element 1"};

  array.Append(element);

  Value copy{VType::kArray};
  copy.Append(Value{});
  copy = array;

  EXPECT_EQ(copy[0].string(), array[0].string());
  EXPECT_EQ(copy.size(), array.size());
}

TEST(BasicArrayTest, MoveAssignment) {
  Value array{VType::kArray};
  Value element{"element 1"};

  array.Append(element);

  Value moved{VType::kArray};
  moved.Append(Value{});
  moved = std::move(array);

  EXPECT_EQ(moved[0].string(), "element 1");
  EXPECT_EQ(array.size(), size_t(0));
}

TEST(BasicArrayTest, ReadWrite) {
  Value array{VType::kArray};

  Value element1{"element 1"};
  Value element2{"element 2"};

  array.Append(element1);
  array.Append(element2);

  ASSERT_TRUE(array.size() == 2);
  EXPECT_EQ(array[0].string(), element1.string());
  EXPECT_EQ(array[1].string(), element2.string());

  Value newElement{"new"};
  array[1] = newElement;

  EXPECT_EQ(array[1].string(), newElement.string());
}

TEST(BasicArrayTest, Erase) {
  Value array{VType::kArray};

  Value element1{"element 1"};
  Value element2{"element 2"};

  array.Append(element1);
  array.Append(element2);

  ASSERT_EQ(array.size(), size_t{2});

  array.Erase(1);

  ASSERT_EQ(array.size(), size_t{1});
  EXPECT_EQ(array[0].string(), "element 1");
}