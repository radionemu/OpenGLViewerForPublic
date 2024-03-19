/*
    //obj model
    chrono::system_clock::time_point startt = chrono::system_clock::now();

    const char mtlpath[1024] = "";
    obj_opener("untitled.obj", objCollection, mtlpath); //.obj file import
    mtl_opener(mtlpath, mtlCollection); //.mtl file import
    //texture import
    for(int i=0; i<mtlCollection.size(); i++){
        textureCollection.push_back(LoadPNG(mtlCollection[i].texturepath));
    }

    chrono::system_clock::time_point endt = chrono::system_clock::now();
    chrono::milliseconds ms = chrono::duration_cast<chrono::milliseconds>(endt-startt);
    chrono::seconds ss = chrono::duration_cast<chrono::seconds>(endt-startt);
    printf("object import complete. \nexecution time(ns) : %ld ns \nexecution time(ms) : %ld ms \nexecution time(s) : %ld s", (endt-startt), ms.count(), ss.count());


    unsigned int smeshcount = 0;
    for(int i=0; i<objCollection.size(); i++){
        smeshcount += objCollection[i].sMesh.size();
        for(int j=0; j<objCollection[i].sMesh.size(); j++){
            printf("object name : %s \n\tSubMesh name : %s \tpos : %d size %d\n", objCollection[i].objName, objCollection[i].sMesh[j].mtlname, objCollection[i].sMesh[j].vStart, objCollection[i].sMesh[j].vsize);
        }
    }
*/

    /*
    for(int i=0; i<objCollection.size(); i++){
        for(int j=0; j<objCollection[i].sMesh.size(); j++){
            unsigned int sMeshsize = objCollection[i].sMesh[j].vsize;
            unsigned int sMeshpos = objCollection[i].sMesh[j].vStart;
            vao[k] = new VertexArray();
            vbo[k] = new VertexBuffer();
            vbo[k]->FillData(&((float *)(objCollection[i].pv))[sMeshpos*8], sMeshsize*sizeof(float)*8);
            vao[k]->AttribBuffer(*vbo[k], layouts);
            k++;
        }
    }
    */