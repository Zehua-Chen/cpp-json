//
//  setup-dev.js
//  JS
//
//  Created by Zehua Chen on 12/26/18.
//  Copyright © 2018 Zehua Chen. All rights reserved.
//

const childProcess = require('child_process');
const fs = require('fs');
const path = require('path');

const dependenciesFolder = "dependencies";

function checkout(cotainingFolder, url, tag) {
  
  process.chdir(cotainingFolder);
  const folderName = path.parse(url).name;
  
  if (!fs.existsSync(folderName)) {
    childProcess.execSync("git clone " + url);
    process.chdir(folderName);
    childProcess.execSync("git checkout " + tag);
  }
}

// Change to project's root dir
process.chdir("../")

// Make sure "dependencies" folder exits
if (!fs.existsSync(dependenciesFolder)) {
  console.log("Create directory " + dependenciesFolder);
  fs.mkdirSync(dependenciesFolder);
}

// Checkout Google test 1.8.1
checkout(
  dependenciesFolder, 
  "https://github.com/google/googletest.git", 
  "release-1.8.1"
);