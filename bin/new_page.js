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

let finalSrcPath = srcPath + ".re";
let finalLinkPath = linkPath + ".js";

if (!fs.existsSync(finalSrcPath)) {
  // Write the BS file in place
  fs.writeFileSync(finalSrcPath, "// Add your page code here!\n");
}

if (!fs.existsSync(finalLinkPath)) {
  // Link the JS file into place
  fs.symlinkSync(
    path.relative(path.dirname(finalLinkPath), finalSrcPath),
    finalLinkPath
  );
}

console.log(`Done! ${finalLinkPath} should now point to ${finalSrcPath}`);
