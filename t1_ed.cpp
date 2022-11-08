#include <iostream>
typedef void *(* callback)(void *, void *);
/*
 * enum
 *
 * headwer
 *
 * */

enum enum_tipo{
    line,
    tab,
    reftab,
    header,
    refheader,
    root
};


struct lista {
    void * info;
    enum_tipo tab;
    lista *next = nullptr;

    lista *before = nullptr;
    lista * first = nullptr;

    lista * last = nullptr;

    void * callback(callback f, void *data = nullptr){
        for (lista * item = this->first; item != nullptr ; item=item->next) {
            void * retorno =  f(item, data);
            if(retorno != nullptr) {
                return retorno;
            }
        }

        return nullptr;
    }

    void nextSibling(lista *item){

        if(this->next == nullptr){
            this->next = item;
            item->before = this;
        }
        else {
            auto oldNextSibling = this->next;
            oldNextSibling->before = item;
            this->next = item;

            item->next = oldNextSibling;
            item->before = this;
        }

    }

    void beforeSibling(lista  *item){
        if(this->before == nullptr){

            this->before = item;
            item->next = this;

        } else {
            auto oldBeforeSibling = this->before;
            oldBeforeSibling->next = item;
            this->before = item;

            item->next = this;
            item->before = oldBeforeSibling;
        }
    }

    void pushBack(lista *item){

        if(this->last == nullptr){

            this->first = item;
            this->last = item;

        }
        else {

            this->last->nextSibling(item);
            this->last = item;

        }

    }

    void pushFront(lista *item){

        if(this->first == nullptr){

            this->first = item;
            this->last = item;

        }
        else {

            this->first->beforeSibling(item);
            this->first = item;
            std::cout << this->first << std::endl;


        }

    }

    void removeSelf(void *f(void *) = nullptr){
        std::cout << "???95";
        if(this->next){
            this->next->before = this->before;
        }

        if(this->before){
            this->before->next = this->next;
        }

        if(f!= nullptr){
            std::cout << "AAAAAaaa" << std::endl;
            f(this);
        }/*
*/
        //delete this;

    }

    void removeChild(lista  *l, void *f(void *) = nullptr){
        std::cout << "???1122";
        if(this->first == l){
            this->first = l->next;
        }

        if(this->last == l){
            this->last = l->before;
        }
        std::cout << "???119";


        l->removeSelf(f);


    }
};

lista * searchBranchByName(lista *db, std::string branchName);

struct BranchInfo{

    std::string name;
    lista fields;

    BranchInfo(lista * db, std::string name, std::string fields[], int length){
        this->name = name;
        for (int i = 0; i<length; i++){
            if(fields[i][0] == '#' || fields[i][0] == '$'){
                auto referencedBranch = searchBranchByName(db, fields[i].substr(1));
                if(referencedBranch== nullptr){
                    std::cout <<"Lista Referenciada Não encontrada" << std::endl;
                    break ;//TODO tratar delete
                } else {
                    auto addfield= new std::string { fields[i] };

                    this->fields.pushBack(new lista {addfield});
                    std::cout << *static_cast<std::string *>(this->fields.last->info) << std::endl;
                    this->fields.last->pushBack(new lista {&(static_cast<BranchInfo *>(referencedBranch->info)->fields)});
                    continue;
                }
            }

            auto addfield= new std::string { fields[i] };
            this->fields.pushBack(new lista {addfield});

        }

    }

};

struct querydata{
    int fieldIndice;
    std::string query;
};

lista *createHeader(){
    auto header = new lista{};
}

lista * createDB(){
        return new lista;
}

void addTable(lista *db, std::string nameTable, std::string fields[], int length){
    if(fields != nullptr) {


        auto *table = new lista;
        table->info = new BranchInfo(db, nameTable, fields, length);
        std::cout << *static_cast<std::string *>(static_cast<BranchInfo *>(table->info)->fields.last->info) << " Binfonew"<<std::endl;
        db->pushBack(table);

        delete[] fields;
    }
}

void * returnPointerIfName(void *item, void *string){
    if(static_cast<BranchInfo *>(static_cast<lista *>(item)->info)->name == *(static_cast<std::string *>(string)))
        return item;
    return nullptr;
}

lista * searchBranchByName(lista *db, std::string branchName){
    auto pt = db->callback(returnPointerIfName, &branchName);
    return static_cast<lista *>(pt);
}

void * returnPointerIfId(void *item, void *id){
    if(
            *(static_cast<int *>(
                    (static_cast<lista *>(item)->info)
                    )
              ) ==
            *(static_cast<int *>(id))){
        return item;
    }

    return nullptr;
}

lista * searchId(lista *db, std::string branchName, int id){
    lista * pt = searchBranchByName(db, branchName);

    if(pt == nullptr){
        std::cout << "Branch Name " << branchName << " Not found" << std::endl;
        return pt;
    }

    pt = static_cast<lista * >(pt->callback(returnPointerIfId, &id));

    if(pt == nullptr){
        //std::cout <<  branchName << "--> nullptr" << std::endl;
    }

    return pt;
}

void * searchPointer(void *itemv, void *pointerv){

    auto item = static_cast<lista *>(itemv);

    if(item && item->first == nullptr){
        return searchPointer(item->info, pointerv);
    }
    auto pointer = static_cast<lista *>(pointerv);
    if(item == pointer){
        return item;
    }
    return nullptr;
}

void addLine(lista *db, lista *table, int id = 0){
    bool erro = false;
    auto newBranch = new lista;
    //std::cout << static_cast<BranchInfo * >(table->info)->name << std::endl;
    //Define o ID a Ser Adicionado
    if(id == 0){
        if(table->last != nullptr){

            id = 1+*(static_cast<int *>(table->last->info));
        } else {
            id = 1;
        }
    }

    //Verifica se o ID Já existe, caso sim, trata como erro

    if(searchId( db, static_cast<BranchInfo *>(table->info)->name, id ) != nullptr){
        erro = true;
        std::cout << "ERRO: : ID já consta na base de dados" << std::endl;
    }
    else {

        newBranch->info = new int{id};//Define o ID (Padrão para todos os dados)

        //Provável parte mais importante da função
        for (auto item = static_cast<BranchInfo *>(table->info)->fields.first->next; item != nullptr; item = item->next) {

            //Percorre a lista de Fields(Nome das "Colunas") começando do 2º, o 1º é o ID
            auto stringF = (*(static_cast<std::string *>(item->info)));

            //std::cout << stringF <<":"<< std::endl;
            switch (stringF[0]) {
                case '#':{//Referencia para Linha

                    int foreing;
                    std::cin >> foreing;
                    std::cout << foreing;

                    lista *pt = searchId(db, stringF.substr(1), foreing);//Verifica se o ID inserido existe na tabela

                    if(pt == nullptr){
                        std::cout << "Item " << foreing << " não encontrado em " << stringF << std::endl;
                        erro = true;
                    }else {
                        newBranch->pushBack(new lista{pt});
                    }

                    break ;
                }

                case '$':{//Lista Contida

                    auto lista_contida = new lista {static_cast<BranchInfo *>(searchBranchByName(db, stringF.substr(1))->info)};
                    std::cout << "Field address AAAAA" << searchBranchByName(db, stringF.substr(1)) << std::endl;
                    newBranch->pushBack(lista_contida);
                    break;
                }

                default:{//StringComum
                    std::string input;
                    std::cin >> input;
                    newBranch->pushBack(new lista{new std::string{input}});
                }
            }

            if(erro){
                break ;
            }
        }
    }

    if(erro){

        //tratar aqui de excluir a newBranch
        //deleteBrach(static_cast<BranchInfo *>(table->info)->fields, newBranch);

    }
    else {
        if(id == 2 || id == 3){std::cout << "<|--" << newBranch << std::endl;}
        table->pushBack(newBranch);//Insere a nova linha criada na table
    }

}

void * addReference(lista *root, lista *list, lista *pointer){

    //Verifica se a tabela a referenciar existe
    auto ref = searchBranchByName(root, static_cast<BranchInfo *>(list->info)->name);

    if(ref == nullptr){
        return list;
    }

    //Necessário Criar um search
    ref = static_cast<lista *>(ref->callback(searchPointer, pointer));

    if(ref == nullptr){
        return list;
    }

    ref->pushBack(new lista{pointer});
    return pointer;
}

int searchField(lista *field, std::string FIELD){
    if(field){


        int address = 0;
        for (lista * i = field->first; i != nullptr ; i = i->next) {
            if(*static_cast<std::string *>(i->info) == FIELD){
                return address;
            }
            std::cout <<*static_cast<std::string *>(i->info) << " Fields "<< *static_cast<std::string *>(field->last->info)<< std::endl;
            address++;
        }
        return -1;

    }
    return -1;
}

void * printList(void *itemv, void *fieldsv){
    if(itemv == nullptr){
        return nullptr;
    }

    auto item = static_cast<lista *>(itemv);
    auto fields = static_cast<lista *>(fieldsv);

    if(!item->first){//Lista Referenciadora
        printList(item->info, fieldsv);
        return nullptr;
    }


    std::cout << "{" << std::endl;
    std::cout << "\"" << *static_cast<std::string *>(fields->first->info) << "\":" << *static_cast<int *>(item->info) <<std::endl;
    auto fieldItem = fields->first->next;
    for (auto i = item->first; i != nullptr; i=i->next) {

        switch ((*static_cast<std::string *>(fieldItem->info))[0]) {
            case '#':{
                std::cout << "\""<<*static_cast<std::string *>(fieldItem->info) << "\":";
                printList(static_cast<lista *>(static_cast<lista *>(i)->info), static_cast<lista *>(fieldItem->first->info));
                break;
            }
            case '$':{
                std::cout << "\""<<*static_cast<std::string *>(fieldItem->info) << "\":";
                static_cast<lista *>(i)->callback(printList, &static_cast<BranchInfo *>(i->info)->fields);
                break ;
            }

            default:{
                std::cout << "\""<<*static_cast<std::string *>(fieldItem->info) << "\":\"" << *static_cast<std::string *>(i->info) << "\"" << std::endl;
            }
        }

        fieldItem=fieldItem->next;
    }

    std::cout << "}," << std::endl;
    return nullptr;
}

void * search(void *listav, void *datav){
    auto list = static_cast<lista *>(listav);
    auto data = static_cast<querydata *>(datav);


    if(data->fieldIndice == 0){
        if(*static_cast<int *>(list->info) == std::stoi(data->query)){

            return list;
        } else {
            return nullptr;
        }
    }
    auto item = list->first;
    for (int i = 1; i < data->fieldIndice; ++i) {
        item = item->next;
    }

    //std::cout << *static_cast<std::string *>(item->info) << ":" << data->query << std::endl;


    if(*static_cast<std::string *>(item->info) == data->query){
        std::cout << "Teste" << *static_cast<std::string *>(item->info) << std::endl;
        return list;
    }

    return nullptr;



}

void * searchLists(void *Listv){
    if(Listv){
        auto list = static_cast<lista *>(Listv);
        auto result = new lista{list->info};


        std::string F;
        std::cin >> F;

        std::cout << *static_cast<std::string *>(static_cast<BranchInfo *>(list->info)->fields.last->info) << std::endl;
        int index = searchField(&(static_cast<BranchInfo *>(list->info)->fields), F);
        std::cout << "Coluna " << index <<" " << F<<std::endl;
        if(index >= 0){
            querydata qdata;
            qdata.fieldIndice = index;
            std::string querys;
            std::cin >> querys;
            qdata.query = querys;

            for (auto i = list->first; i != nullptr; i = i->next) {
                auto s = search(i, &qdata);
                if(search(i, &qdata) != nullptr){
                    result->pushBack(new lista {s});
                }
            }
        }

        return result;

    }
    std::cout << "Lista Não encontrada" << std::endl;
    return nullptr;
}

void * addResultfEach(void *resultItem, void*listv){
    auto item = static_cast<lista *>(resultItem)->info;
    auto list = static_cast<lista *>(listv);

    auto s = list->callback(searchPointer, item);

    if(!s){
        list->pushBack(new lista{item});
    }

    return nullptr;
}

void * addResult(void *resultv, void *listv){
    auto result = static_cast<lista *>(resultv);
    auto list = static_cast<lista *>(listv);

    if(result->info == list->info){
        result->callback(addResultfEach, list);
    } else {
        std::cout << "Listas Incompátiveis" << std::endl;
    }

    return nullptr;

}
lista * db;


int main(){
    db = createDB();

    //Criação de Tabelas
    addTable(db, "CURSO",  new std::string[] {"ID", "NOME"}, 2);
    addTable(db, "PROFESSOR",  new std::string[] {"ID", "NOME"}, 2);
    addTable(db, "DISCIPLINA",  new std::string[] {"ID", "NOME", "#CURSO"}, 3);
    addTable(db, "ALUNO",  new std::string[] {"ID", "NOME", "#CURSO"}, 3);
    std::cout << *static_cast<std::string *>(static_cast<BranchInfo *>(searchBranchByName(db, "ALUNO")->info)->fields.last->info) << std::endl;

    addTable(db, "TURMA",  new std::string[] {"ID", "NOME", "#PROFESSOR", "#DISCIPLINA", "$ALUNO"}, 5);


    //Adding Curso
    addLine(db, db->first);
    addLine(db, db->first);
    addLine(db, db->first);



    addLine(db, searchBranchByName(db, "DISCIPLINA"));
    addLine(db, searchBranchByName(db, "PROFESSOR"));
    addLine(db, searchBranchByName(db, "TURMA"));
    addLine(db, searchBranchByName(db, "ALUNO"));


    //std::cout << searchBranchByName(db, "ALUNO")->info << std::endl;



    //addLine(db,searchBranchByName(db, "TURMA")->first->next->next)
    ///std::cout << "Printando lista de listas" << std::endl;
    //printList(&static_cast<BranchInfo *>(db->first->next->next->info)->fields, db->first->next->next->first);
    //searchBranchByName(db, "DISCIPLINA")->callback(printList, &static_cast<BranchInfo *>(searchBranchByName(db, "DISCIPLINA")->info)->fields);
    std::cout << std::endl << std::endl << std::endl;
    searchBranchByName(db, "TURMA")->callback(printList, &static_cast<BranchInfo *>(searchBranchByName(db, "TURMA")->info)->fields);

    std::cout << searchField(&static_cast<BranchInfo *>(searchBranchByName(db, "TURMA")->info)->fields, "#PROFESSOR") << std::endl;

    //addResult(r, (searchBranchByName(db, "ALUNO")->first->first->next->next->next));
    std::cout << "busca" << std::endl;

    auto r2 = searchLists(searchBranchByName(db, "ALUNO"));
    auto r = searchLists(searchBranchByName(db, "ALUNO"));
    static_cast<lista *>(r)->callback(printList,&static_cast<BranchInfo *>(static_cast<lista *>(r)->info)->fields );
    //printList(static_cast<lista *>(r)->first, &static_cast<BranchInfo *>(static_cast<lista *>(r)->info)->fields);

    //searchBranchByName(db, "TURMA")->callback(printList, &static_cast<BranchInfo *>(searchBranchByName(db, "TURMA")->info)->fields);


    //searchBranchByName(db, "CURSO")->callback(printList, &static_cast<BranchInfo *>(searchBranchByName(db, "CURSO")->info)->fields);
    //searchBranchByName(db, "CURSO")->removeChild(searchBranchByName(db, "CURSO")->first, nullptr);
    //searchBranchByName(db, "CURSO")->callback(printList, &static_cast<BranchInfo *>(searchBranchByName(db, "CURSO")->info)->fields);

    delete db->first;
    delete db;

}