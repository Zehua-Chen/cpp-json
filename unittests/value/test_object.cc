//
//  BasicObjectTest.cpp
//  CPP
//
//  Created by Zehua Chen on 12/31/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "json/value/basic_value.h"

using std::string;
using std::string_view;

using std::cout;
using std::endl;

using json::BasicValue;

using Value = BasicValue<char>;
using VType = Value::Type;

TEST(ObjectTest, Construction) {
  Value object{VType::kObject};
  EXPECT_EQ(object.type(), BasicValue<char>::Type::kObject);
  EXPECT_TRUE(object.IsObject());
}

TEST(ObjectTest, CopyConstruction) {
  Value object{VType::kObject};
  Value name{"jackson"};

  object["name"] = name;

  BasicValue<char> copy = object;

  ASSERT_EQ(copy["name"].type(), VType::kString);
  EXPECT_EQ(object["name"].string(), copy["name"].string());
}

TEST(ObjectTest, MoveConstruction) {
  Value object{VType::kObject};
  Value name{"jackson"};

  object["name"] = name;

  BasicValue<char> moved = std::move(object);

  ASSERT_EQ(moved.type(), VType::kObject);
  ASSERT_EQ(moved["name"].type(), VType::kString);
  EXPECT_EQ(moved["name"].string(), "jackson");

  EXPECT_EQ(object.size(), size_t(0));
}

TEST(ObjectTest, CopyAssignment) {
  Value object{VType::kObject};
  Value name{"jackson"};

  object["name"] = name;

  Value copy{VType::kObject};
  copy["random data"] = Value{};

  copy = object;

  ASSERT_EQ(object.type(), VType::kObject);
  ASSERT_EQ(copy.type(), VType::kObject);
  ASSERT_EQ(copy["name"].type(), VType::kString);
  ASSERT_EQ(object["name"].type(), VType::kString);

  EXPECT_EQ(object["name"].string(), copy["name"].string());
}

TEST(ObjectTest, MoveAssignment) {
  Value object{VType::kObject};
  Value name{"jackson"};

  name.set_string("jackson");
  object["name"] = name;

  Value moved{VType::kObject};
  moved["random data"] = Value{};
  moved = std::move(object);

  EXPECT_EQ(moved["name"].string(), "jackson");
  EXPECT_EQ(object.size(), size_t(0));
}

TEST(ObjectTest, ReadWriteWithMethods) {
  Value object{VType::kObject};
  Value name{"jackson"};

  object.Set("name", name);

  ASSERT_TRUE(object.size() == 1);
  ASSERT_TRUE(object.Contains("name"));

  EXPECT_EQ(object.Get("name").string(), name.string());
}

TEST(ObjectTest, ReadWriteWithSubscripts) {
  Value object{VType::kObject};
  Value name{"jackson"};

  object["name"] = name;

  ASSERT_TRUE(object.size() == 1);
  ASSERT_TRUE(object.Contains("name"));

  EXPECT_EQ(object.Get("name").string(), name.string());

  Value newName{"peter"};
  object["name"] = newName;

  ASSERT_TRUE(object.size() == 1);
  ASSERT_TRUE(object.Contains("name"));

  EXPECT_EQ(object.Get("name").string(), newName.string());
}

TEST(ObjectTest, Erase) {
  Value object{VType::kObject};
  Value name{"jackson"};

  object["name"] = name;

  ASSERT_TRUE(object.Contains("name"));

  object.Erase("name");

  ASSERT_FALSE(object.Contains("name"));
}