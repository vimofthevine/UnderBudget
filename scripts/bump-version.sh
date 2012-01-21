#! /bin/bash

echo -n "Specify the new version number: "
read -e VERSION

echo Bumping version to $VERSION
sed -i "s/VERSION\ =\ \".*\"/VERSION = \"$VERSION\"/" src/com/vimofthevine/underbudget/Application.java
sed -i "s/pkgver=.*/pkgver=$VERSION/" PKGBUILD
sed -i "s/version\" value=\".*\"/version\" value=\"$VERSION\"/" build.xml

