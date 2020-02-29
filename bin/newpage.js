#!/usr/bin/env node

const fs = require("fs");
const mkdirp = require("mkdirp");
const path = require("path");

const pagesDirExists = fs.existsSync("pages");

let pagePath = process.argv[2];
if (!pagePath) {
  console.log(
    "Please give me the path to a page you want to create! like: posts/[pid]"
  );
  process.exit(1);
}

function capitalize(str) {
  return str.charAt(0).toUpperCase() + str.substring(1);
}

function makeBSBSafe(path) {
  let noSquareBrackets = path
    .replace(/\[/g, "Dynamic_")
    .replace(/\]/g, "")
    .replace(/\.\.\./g, "All_");
  let splitted = noSquareBrackets.split("/");
  let filename = splitted[splitted.length - 1];
  splitted[splitted.length - 1] = capitalize(filename);
  return splitted.join("/");
}

let srcPath = makeBSBSafe(`src/pages/${pagePath}`);
let linkPath = `pages/${pagePath}`;

if (!fs.existsSync(srcPath)) {
  let splitted = srcPath.split("/");
  mkdirp.sync(splitted.slice(0, splitted.length - 1).join("/"));
}

if (!fs.existsSync(linkPath)) {
  let splitted = linkPath.split("/");
  mkdirp.sync(splitted.slice(0, splitted.length - 1).join("/"));
}

let finalSrcPath = srcPath + ".bs.js";
let finalLinkPath = linkPath + ".js";

if (!fs.existsSync(srcPath + ".re")) {
  // Write the BS file in place
  fs.writeFileSync(
    srcPath + ".re",
    `
open React;
open Next;

[@react.component]
let default = () => {
  <div>"Hello"->string</div>;
};
`
  );
}

if (!fs.existsSync(finalLinkPath)) {
  fs.writeFileSync(
    finalLinkPath,
    `
import Page from "${path.relative(path.dirname(finalLinkPath), finalSrcPath)}";
export default Page;
`
  );
}

console.log(`Done! ${finalLinkPath} should now point to ${finalSrcPath}`);
