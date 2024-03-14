/*
 * dbsettings.cpp - implementation of DiskBrowserDialog's DbSettings class.
 *
 * Copyright 2023 D.Caputi
 */
#include "diskbrowser/dbsettings.h"

DbSettings::DbSettings()
{
}

DbSettings::~DbSettings()
{
}

bool DbSettings::isEmpty()
{
    if (_dirty)
        return false;

    return //_appData.isEmpty() && c/o any settings cached in the main settings
            // TBD: more than this plus don't c/o above
           _dirMap.isEmpty();
}

void DbSettings::clone(DbSettings& other)
{
    _dirMap = other._dirMap;
    _diskPic = other._diskPic;   // TBD: more than this (?) ...hang on, is this used?
    _dirty = true;
}

void DbSettings::merge(DbSettings& other)
{
    _dirMap.insert(other.getDirMap());
    _dirty = true;
}
