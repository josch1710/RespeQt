#ifndef PICSOURCETYPE_H
#define PICSOURCETYPE_H

enum DbDataSource
{
    DbData_none = 0,
    DbData_appSettings,
    DbData_subDirJson,
    DbData_appFolderJson
};

enum PicSourceType
{
    PicSource_none = 0,
    PicSource_floppy,
    PicFromFname_base,  // identical basename
    PicFromFname_index, // matching index id
    PicFromFname_dir,   // dir has respeqt_db.png
    PicFromJson_global, // [db] pic
    PicFromJson_dir,    // [<escaped dir>] pic
    PicFromJson_disk    // [<escaped dir>] <escaped disk>\pic
};

#endif // PICSOURCETYPE_H
