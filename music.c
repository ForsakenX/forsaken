#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sound.h"
#include "oct2.h"

int16_t CurrentLevel = -1;
char MusicPath[MAX_PATH];

struct music_buffer_t {
    ALuint id[16];
    ALuint source;
    char path[MAX_PATH];
    int current_section;
    OggVorbis_File vf;
    vorbis_info *vi;
    ALuint released[16];
    ALint count;
    bool eof;
};

music_buffer_t *music_load(music_buffer_t *buffer, const char *path)
{
    ALenum format;
    FILE *fp;
    strncpy(buffer->path,path,MAX_PATH-1);
    fp = file_open(buffer->path, "rb");
    if(!fp){
       DebugPrintf(stderr, "could not open file %s", buffer->path);
       return 0;
    }
    if(ov_open_callbacks(fp, &buffer->vf, NULL, 0, OV_CALLBACKS_DEFAULT)<0){
       DebugPrintf(stderr, "input does not appear to be in an ogg bitstream");
       return 0;
    }
    buffer->vi = ov_info(&buffer->vf, -1);
    int i;
    for(i = 0;i<16;i++){
       long pos = 0;
       while(pos < sizeof(pcmout)){
          long ret = ov_read(&buffer->vf, pcmout+pos, sizeof(pcmout)-pos, 0, 2, 1, &buffer->current_section);
          pos+=ret;
          if(ret == 0){
            break;
          }
       }
       alBufferData(buffer->id[i], AL_FORMAT_STEREO16, pcmout, pos, buffer->vi->rate);
    }
    buffer->eof = false;
    alSourceQueueBuffers(buffer->source, 16, buffer->id); //cole added address of on 2-24-2017
    alSourcePlay(buffer->source);
    return buffer;
}

void music_play(){
    alGetSourcei(music_buffer->source, AL_BUFFERS_PROCESSED, &music_buffer->count);
    alSourceUnqueueBuffers(music_buffer->source, music_buffer->count, music_buffer->released);
    int i;
    for(i = 0;i<music_buffer->count;i++){
      long pos = 0;
      while(pos < sizeof(pcmout)){
        long ret = ov_read(&music_buffer->vf, pcmout+pos, sizeof(pcmout)-pos, 0, 2, 1, &music_buffer->current_section);
        pos+=ret;
        if(ret == 0){
          music_buffer->eof = true;
          break;
        }
     }
     alBufferData(music_buffer->released[i], AL_FORMAT_STEREO16, pcmout, pos, music_buffer->vi->rate);
    }
    alSourceQueueBuffers(music_buffer->source, music_buffer->count, music_buffer->released);
}

void music_cleanup(){
        int i;
        for (i = 0; i < 16; i++){
          alSourcei(music_buffer->id[i], AL_BUFFER, NULL);
        }
        alSourceStopv(1, &music_buffer->source); //may not need
        alDeleteSources(1, &music_buffer->source);
        for (i = 0; i < 16; i++){
            alDeleteBuffers(16, &music_buffer->id[i]);
        }
        alutExit();
}

bool InitMusic(){
    ALenum error;
    if(music_buffer){
        music_cleanup();
        music_buffer = realloc(music_buffer,sizeof(music_buffer_t));
    }else{
        music_buffer = malloc(sizeof(music_buffer_t));
    }
    if(!music_buffer){
        DebugPrintf("sound_load: failed to malloc buffer\n");
        return false;
    }
    // clear error code
    alGetError();
    alGenBuffers(16, music_buffer->id);
    if ((error = alGetError()) != AL_NO_ERROR){
            DebugPrintf("alGenBuffers: %s\n", alGetString(error));
            free(music_buffer);
            return false;
    }
    alGenSources (1, &music_buffer->source);
    return true;
}

char *trackmap(const char *folderpath, const char *filename){
    const size_t length = strlen(folderpath) + strlen(filename) + 1;
    char *path = malloc(sizeof(char) * length);
    snprintf(path, length, "%s%s", folderpath, filename);
    if(folder_exists(path) == 0){
      DebugPrintf("Path to %s missing, creating default\n",path);
      touch_file(path);
    }
    FILE *f = file_open(path, "r");
    if(!f){
       DebugPrintf(stderr, "could not open file %s", path);
       return NULL;
    }
    const int16_t buf=128;
    int16_t ch = 0;
    int16_t tot = 0;
    while(!feof(f)){
      ch = fgetc(f);
      if(ch == '\n'){
        tot++;
      }
    }
    fclose(f);
    f = file_open(path,"r");
    free(path);
    char line[tot][buf];
    int i = 0;
    while(fgets(line[i], buf, f)){
        // get rid of ending \n from fgets
        line[i][strlen(line[i]) - 1] = '\0';
        i++;
    }
    fclose(f);
    return line[LevelNum];
}

bool MusicLoop(){
    if(music_buffer->eof){
      if(!InitMusic()){
          Msg("InitMusic failed.\n");
          return false;
      }
      music_buffer = music_load(music_buffer,MusicPath);
      if (!music_buffer){
          return false;
      }
    }else{
        if(CurrentLevel != LevelNum  && LoadLevel == true){
            const char *trackname = trackmap("data/sound/music/","music.dat");
            char path[MAX_PATH];
            if(!trackname){
                return false;
            }
            strncpy(&path,trackname,MAX_PATH-1);
            //sprintf(MusicPath,"data\\sound\\music\\%s.ogg", path,MAX_PATH-1);  //couldnt get this to work!!!
            sprintf(&MusicPath,"data/sound/music/%s.ogg", path,MAX_PATH-1);
            LoadLevel = false;
            music_buffer->eof = true;
            CurrentLevel = LevelNum;
        }else{
            music_play();
        }
    }
    return true;
}
