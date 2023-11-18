#ifndef PICSOURCETYPE_H
#define PICSOURCETYPE_H

enum PicSourceType
{
    PicSource_none = 0,
    PicFromFile_base,   // identical basename
    PicFromFile_index,  // matching index id
    PicFromFile_dir,    // dir has respeqt_db.png
    PicFromIni_global,  // [db] pic
    PicFromIni_dir,     // [<escaped dir>] pic
    PicFromIni_disk     // [<escaped dir>] <escaped disk>\pic
};

#endif // PICSOURCETYPE_H
