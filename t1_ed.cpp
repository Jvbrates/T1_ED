#include <iostream>

typedef void *(* callback)(void *, void *);
int tabv = 0;
enum enum_tipo{
    root,
    branch,
    refbranch,
    line,
    headerrefid,
    headerstr,
    headerrefbranch,
    refline,
    headerbranch,
    str


};
void auxiliotipo(void *);
void * deleteRecursive(void *listav, void *nada = nullptr);
struct lista {
    void * info;
    enum_tipo tipo;

    lista *next = nullptr;
    lista *before = nullptr;
    lista * first = nullptr;
    lista * last = nullptr;

    void * callback(void* f(void *, void *), void *data = nullptr){
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
        }

    }

    void removeSelf(void *f(void *) = nullptr){

        if(this->next){
            this->next->before = this->before;
        }

        if(this->before){
            this->before->next = this->next;
        }

        if(f!= nullptr){
            f(this);
        }

        deleteRecursive(this);

    }

    void removeChild(void  *lv, void *f(void *) = nullptr){
        auto l = static_cast<lista *>(lv);

        if(this->first == l){
            this->first = l->next;
        }

        if(this->last == l){
            this->last = l->before;
        }

        l->removeSelf(f);
    }


};

void auxiliotipo(void *l){
    switch(static_cast<lista *>(l)->tipo){
        case refline:
            std::cout << "Tipo refline" << std::endl;
            break;
        case refbranch:
            std::cout << "Tipo refbranch" << std::endl;
            break;
        case branch:
            std::cout << "Tipo branch" << std::endl;
            break;
        case line:
            std::cout << "Tipo line" << std::endl;
            break;
        case str:
            std::cout << "Tipo str" << std::endl;
            break;
        case headerrefbranch:
            std::cout << "Tipo headerrefBranch" << std::endl;
            break;
        case headerbranch:
            std::cout << "Tipo headerbranch" << std::endl;
            break;
        case headerrefid:
            std::cout << "Tipo headerrefid" << std::endl;
            break;
        case root:
            std::cout << "Tipo root" << std::endl;
            break;
        case headerstr:
            std::cout << "Tipo headerstr" << std::endl;
            break;
    }
}

lista * searchBranchByName(lista *db, std::string branchName);

lista *header(lista *db, std::string name, std::string fields[], int length){
    auto header = new lista{new std::string { name }, headerbranch};
    bool erro  = false;
    for (int i = 0; i<length; i++){
        switch (fields[i][0]) {
            case '$':
            {
                auto referencedBranch = searchBranchByName(db, fields[i].substr(1));
                if(referencedBranch){
                    header->pushBack(new lista{
                            new std::string{fields[i]},
                            headerrefbranch
                    });


                }else{
                    std::cout <<"Lista Referenciada Não encontrada" << std::endl;
                    erro = true;
                }

                break ;
            }
            case '#':{
                auto referencedBranch = searchBranchByName(db, fields[i].substr(1));
                if(referencedBranch){
                    header->pushBack(new lista{
                            new std::string{fields[i]},
                            headerrefid
                    });

                }else{
                    std::cout <<"Lista Referenciada Não encontrada" << std::endl;
                    erro = true;
                }

                break ;
            }
            default: {
                header->pushBack(new lista{new std::string{fields[i]}, headerstr});
                break;
            }
        }
    }

    if(erro){std::cout << "Algo de errado" << std::endl;}
    return header;
}

struct querydata{
    int fieldIndice;
    lista * resultado;
    std::string query;
    std::string field;
    enum_tipo tipo_field;
};

lista * createDB(){
    return new lista{nullptr, root};
}

void addTable(lista *db, std::string nameTable, std::string fields[], int length){
    if(fields != nullptr) {
        auto table = new lista{

                header(db, nameTable, fields, length),
                branch
        };

        db->pushBack(table);
        delete[] fields;
    }
}

void * returnPointerIfName(void *branch, void *string){//verBranchName
    auto name = *static_cast<std::string *>(static_cast<lista *>(static_cast<lista *>(branch)->info)->info);
    if(name == *(static_cast<std::string *>(string)))
        return branch;
    return nullptr;
}

lista * searchBranchByName(lista *root, std::string branchName){
    auto pt = root->callback(returnPointerIfName, &branchName);

    if(!pt){
        std::cout << "Lista não encontrada" << std::endl;
    }
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

lista * searchIdinBranch(lista *root, std::string branchName, int id){
    lista * pt = searchBranchByName(root, branchName);

    if(pt == nullptr){
        std::cout << "Branch Name " << branchName << " Not found" << std::endl;
        return pt;
    }

    pt = static_cast<lista * >(pt->callback(returnPointerIfId, &id));

    return pt;
}

void * searchPointer(void *itemv, void *pointerv){

    auto item = static_cast<lista *>(itemv);

    if(item && item->tipo == refline){
        if(item->info == pointerv){
            return item;
        }
        return searchPointer(item->info, pointerv);

    }
    auto pointer = static_cast<lista *>(pointerv);
    if(item == pointer){
        return item;
    }
    return nullptr;
}

void addLine(lista *db, lista *branchi, int id = 0){
    bool erro = false;
    auto newBranch = new lista{nullptr, line};

    //Define o ID a Ser Adicionado
    if(id == 0){
        if(branchi->last != nullptr){

            id = 1+*(static_cast<int *>(branchi->last->info));
        } else {
            id = 1;
        }
    }

    //Verifica se o ID Já existe, caso sim, trata como erro
    if(searchIdinBranch( db, *static_cast<std::string *>(static_cast<lista *>(branchi->info)->info), id ) != nullptr){
        erro = true;
    }
    else {

        newBranch->info = new int{id};//Define o ID (Padrão para todos os dados)

        //Provável parte mais importante da função
        for (auto item = static_cast<lista *>(branchi->info)->first->next; item != nullptr; item = item->next) {

            //Percorre a lista de Fields(Nome das "Colunas") começando do 2º, o 1º é o ID
            auto stringF = (*(static_cast<std::string *>(item->info)));

            //            /std::cout << stringF <<":"<< std::endl;
            switch (item->tipo) {
                case headerrefid:{//Referencia para Linha

                    int foreing;
                    std::cin >> foreing;

                    lista *pt = searchIdinBranch(db, stringF.substr(1), foreing);//Verifica se o ID inserido existe na tabela

                    if(pt == nullptr){
                        std::cout << "Item " << foreing << " não encontrado em " << stringF << std::endl;
                        erro = true;
                    }else {
                        newBranch->pushBack(
                                new lista
                                {static_cast<lista *>(searchBranchByName(db, stringF.substr(1))->info),
                                 refbranch, nullptr, nullptr, new lista{pt, refline}
                                });


                    }

                    break ;
                }

                case headerrefbranch:{//Lista Contida

                    auto lista_contida = new lista {
                            static_cast<lista *>(searchBranchByName(db, stringF.substr(1))->info),
                            refbranch};

                    newBranch->pushBack(lista_contida);
                    break;
                }

                default:{//StringComum
                    std::string input;
                    std::cin >> input;
                    newBranch->pushBack(new lista{new std::string{input}, str});
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

        branchi->pushBack(newBranch);//Insere a nova linha criada na table
    }

}

int searchField(lista *field, std::string FIELD){
    if(field){


        int address = 0;
        for (lista * i = field->first; i != nullptr ; i = i->next) {

            if(*static_cast<std::string *>(i->info) == FIELD){
                return address;
            }
            address++;
        }
        return -1;

    }
    return -1;
}

void printBranch(lista *Branch);

void * printList(void *itemv, void *fieldsv){

    if(itemv == nullptr){
        std::cout << "}" << std::endl;
        return nullptr;
    }

    auto item = static_cast<lista *>(itemv);
    auto fields = static_cast<lista *>(fieldsv);

    if(item->tipo == refline){//Lista Referenciadora
        printList(item->info, fields);
        return nullptr;
    }

    //auxiliotipo(fields);
    std::cout <<std::string(tabv, '\t') << "{"  << std::endl;
    std::cout <<std::string(tabv, '\t')<< "\"" << *static_cast<std::string *>(fields->first->info) << "\":" << *static_cast<int *>(item->info) <<std::endl;

    auto fieldItem = fields->first->next;
    for (auto i = item->first; i != nullptr; i=i->next) {
        switch (fieldItem->tipo) {

            case refbranch:{
                std::cout <<std::string(tabv, '\t')<< "\""<<*static_cast<std::string *>(fieldItem->info) << "\":";
                static_cast<lista *>(i)->callback(printList, static_cast<lista *>(i->info)->info);
                break ;
            }

            case headerstr:{
                std::cout <<std::string(tabv, '\t')<< "\""<<*static_cast<std::string *>(fieldItem->info) << "\":\"" << *static_cast<std::string *>(i->info) << "\"" << std::endl;
                break ;
            }
            case headerrefbranch:
            case headerrefid:{
                printBranch(i);
                std::cout << std::endl;
                break ;
            }
            default:{
                return nullptr;
            }

        }

        fieldItem=fieldItem->next;
    }

    std::cout<<std::string(tabv, '\t') << "}" <<std::endl;
    return nullptr;
}

void printBranch(lista *Branch){
    if(Branch) {
        std::cout << std::endl;

        Branch->info &&std::cout << std::string(tabv, '\t') << *static_cast<std::string *>(static_cast<lista *>(Branch->info)->info) << ": {" << std::endl;
        tabv++;
        Branch->callback(printList, static_cast<lista *>(Branch->info));
        tabv--;
        Branch->info &&std::cout << "}";

        if (!Branch->info) {
            std::cout << "Lista Vazia" << std::endl;
        }
    }
}

void * searchLists(void *Listv, lista *r, std::string que, std::string fiel);

void * search(void *listav, void *datav){
    auto list = static_cast<lista *>(listav);
    auto data = static_cast<querydata *>(datav);
    if(list->tipo == refline){
        search(list->info, data);
        return nullptr;
    }

    lista * compar = list->first;
    if(data->fieldIndice == 0){
        if(std::stoi(data->query) == *static_cast<int *>(list->info)){
            data->resultado->pushBack(new lista {list, refline});
        }

        return nullptr;
    }
    for(int i = 0; i< (data->fieldIndice)-1; i++) {
        if (!(compar->next)) {
            break;
        }
        compar = compar->next;
    }
    switch (compar->tipo) {
        case str:{
            if((*static_cast<std::string *>(compar->info)).find(data->query)  != std::string::npos){
                data->resultado->pushBack(new lista {list, refline});

            }
            return nullptr;
            break ;
        }
        case refbranch:{
            if(compar->last == nullptr){


                auto buscaid = new lista {nullptr, refbranch};
                searchLists(compar, buscaid, data->query, data->field);
                if(buscaid->first){
                    data->resultado->pushBack(new lista {list, refline});
                }
                return nullptr;
            }


            searchLists(compar, data->resultado, data->query, data->field);

            return nullptr;
        }

        default:{
            return nullptr;
        }
    }



    return nullptr;



}

void * searchLists(void *Listv, lista *r, std::string que = {'\0'}, std::string fiel = {'\0'}){
    if(Listv){
        auto list = static_cast<lista *>(Listv);

        //auto result = new lista{list->info};

        std::string F;
        if(fiel[0] == '\0'){
            std::cin >> F;
        }else{
            F = fiel;
        }

        r->info = list->info;
        int index = searchField(static_cast<lista *>(list->info), F);

        auto field = static_cast<lista *>(list->info)->first;
        for (auto i = 0; i < index; ++i) {
            field= field->next;
        }

        if(index >= 0){

            switch (field->tipo) {
                case headerstr:{
                    r->info = list->info;
                    std::string query;
                    if(que[0] == '\0') {
                        std::cin >> query;
                    }else {
                        query = que;

                    }


                    querydata q {index, r};
                    q.query = query;
                    //printBranch(list);
                    list->callback(search, &q);

                    printBranch( q.resultado);
                    break;
                }
                case headerrefbranch:
                case headerrefid:{
                    std::string query;
                    std::string field2;

                    if(que[0] == '\0') {
                        std::cin >> field2;
                        std::cin >> query;
                    }else {
                        query = que;
                    }

                    r->info = list->info;
                    querydata q {index, r};
                    q.query = query;
                    q.field = field2;
                    q.tipo_field = field->tipo;

                    list->callback(search, &q);

                    return nullptr;
                    break ;
                }

                default: {
                    return nullptr;
                }
            }

        }else {
            std::cout << "Coluna não encontrada" << std::endl;
        }

        return nullptr;

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

void * deleteSelfinBranch(void *void_pt, void *void_base){
    auto pt = static_cast<lista *>(void_pt);
    auto base = static_cast<lista *>(void_base);
    auto  r = base->callback(searchPointer, pt->info);
    if(r){
        base->removeChild(r);
    }
    return nullptr;
}

void * deleteSelfinBranchRef(void *void_pt, void *void_base){
    auto pt = static_cast<lista *>(void_pt);
    auto base = static_cast<lista *>(void_base);

    for(auto item = base->first; item!= nullptr; item=item->next){
        if(pt->info == item->info){
            base->removeChild(item);}
    }

    return nullptr;
}

struct mergeAux{
    int address;
    lista *insert;
};

void * deleteLists(lista *base, lista *values);

void * deleteListsRef(lista *base, lista *values){

    values->callback(deleteSelfinBranchRef, base);
    return nullptr;
}

void * mergeDelete(void *v_recep_list, void *vo_del){

    auto str_merAux = static_cast<mergeAux *>(vo_del);
    auto recep_list = static_cast<lista *>(v_recep_list);
    auto l = static_cast<lista *>(recep_list->info)->first;

    for (int i = 0; i < str_merAux->address-1; ++i) {
        l=l->next;
    }

    deleteListsRef(l, str_merAux->insert);


    return nullptr;
}

void * deleteLists(lista *base, lista *values){

    if(values->tipo != refbranch){
        std::cout << "A lista com os valores a serem deletados necessita ser uma lista de referencias a lista base";
        return nullptr;
    }
    if(values->info != base->info){//TODO aqui pode mudar

        int search = searchField(static_cast<lista *>(base->info), "$"+(*static_cast<std::string *>(static_cast<lista *>(values->info)->info)));

        if(search != -1){
            mergeAux m {
                    search,
                    values
            };
            base->callback(mergeDelete, &m);
        } else {
            std::cout << "Listas Incompativeis";
        }

        return nullptr;
    }


    values->callback(deleteSelfinBranch, base);
    return nullptr;

}

void * mergeRefBranch(void * void_receptor, void * void_insert);

void * mergeListaContida(void * void_receptor_list, void *v_insert){
    auto str_merAux = static_cast<mergeAux *>(v_insert);
    auto recep_list = static_cast<lista *>(void_receptor_list);
    auto l = static_cast<lista *>(recep_list->info)->first;

    for (int i = 0; i < str_merAux->address-1; ++i) {
        l=l->next;
    }

    mergeRefBranch(l, str_merAux->insert);


    return nullptr;
}

void * compareadd(void *void_ins_item, void *void_rec){
    auto ins_item = static_cast<lista *>(void_ins_item);
    auto rec = static_cast<lista *>(void_rec);
    auto r = rec->callback(searchPointer, ins_item);


    if(!r){
        rec->pushBack(new lista{ins_item->info, refline});
        rec->callback(searchPointer, ins_item);
    }
    return nullptr;
}

void * mergeRefBranch(void * void_receptor, void * void_insert){
    auto receptor = static_cast<lista *>(void_receptor);
    auto insert = static_cast<lista *>(void_insert);

    if(insert->info != receptor->info){

        int search = searchField(static_cast<lista *>(receptor->info), "$"+(*static_cast<std::string *>(static_cast<lista *>(insert->info)->info)));
        if(search != -1){
            mergeAux m {
                    search,
                    insert
            };
            receptor->callback(mergeListaContida, &m);
        } else {
            std::cout << "Listas Incompativeis";
        }
        return nullptr;
    } else {
        std::cout << "Listas Compativeis";
    }

    insert->callback(compareadd, receptor);

    return nullptr;
}

void * deleteRecursive(void *listav, void *nada){
    auto list = static_cast<lista *>(listav);
    switch (list->tipo) {
        case root:{
            std::cout << "Deletando o root" << std::endl;
            list->callback(deleteRecursive);


            break;
        }
        case branch:{
            std::cout << "Tipo info da  branch: ";
            deleteRecursive(list->info);
            std::cout << std::endl;

            std::cout << "Deletando suas sub-listas/linhas" << std::endl;

            list->callback(deleteRecursive);

            std::cout << "Deletando a propria branch" << std::endl;

            std::cout<< std::endl;

            break ;
        }
        case headerbranch:{
            std::cout << std::endl;
            std::cout << "HeaderBranch" << std::endl;
            std::cout << "Deletando o nome da Header de cada lista" << std::endl;
            delete static_cast<std::string *>(list->info);
            std::cout << std::endl;
            std::cout << "Deletando Itens da Header de cada lista" << std::endl;
            list->callback(deleteRecursive);

            std::cout << "Deletando a propria HEADER" << std::endl;


            break ;

        }
        case str:{
            std::cout << "Deletando str "  << std::endl;
            delete static_cast<std::string *>(list->info);

            break ;
        }
        case headerstr:{
            std::cout << "Deletando str na headerstr "  << std::endl;
            delete static_cast<std::string *>(list->info);
            break ;
        }
        case line:{
            std::cout << "Deletando line id " <<  *static_cast<int *>(list->info)<<std::endl;
            delete static_cast<int *>(list->info);
            list->callback(deleteRecursive);

            break ;
        }
        case refbranch:{
            list->callback(deleteRecursive);
            std::cout << "Deletando RefBranch " << std::endl;
            break ;
        }
        case refline:{
            std::cout << "Deletando Refline" << list->first << std::endl;

            break ;
        }
        case headerrefid:{
            std::cout << "Headerfid"<< std::endl;
            std::cout << "Deletando string " << *static_cast<std::string *>(list->info) << std::endl;
            delete static_cast<std::string *>(list->info);

            std::cout << "deletando a propria Headerfid"<< std::endl;

            break ;
        }
        case headerrefbranch:{

            std::cout << "headerrefbranch" << *static_cast<std::string *>(list->info) << std::endl;
            std::cout << "Deletando header " << *static_cast<std::string *>(list->info) << std::endl;
            delete static_cast<std::string *>(list->info);

            std::cout << "Deletando a headerRefBranch" << std::endl;
            break ;
        }


    }

    delete list;
    return nullptr;
}

void * listBranchs(lista *root_){
    if(root_->tipo == root){
        for (auto i = root_->first; i != nullptr ; i = i->next) {
            std::cout << *static_cast<std::string *>(static_cast<lista *>(i->info)->info) << std::endl;
        }
    } else {
        std::cout << "Argumento passado deve ser do tipo root" << std::endl;
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
    addTable(db, "TURMA",  new std::string[] {"ID", "NOME", "#PROFESSOR", "#DISCIPLINA", "$ALUNO"}, 5);


    /*
    int op;
    std::cin >> op;
    switch (op) {
        case 0:{
            std::cout << "___________________________________" << std::endl;
            for (auto i = db->first;  i!= nullptr ; i=i->next) {
                printBranch(i);
                std::cout << "___________________________________" << std::endl;
            }
            break ;
        }

        case 1:{//Search
            auto r = new lista{nullptr, refbranch};
            searchLists(searchBranchByName(db, "DISCIPLINA"), r);
            printBranch(r);
            break ;
        }
        case 2:{
            auto r = new lista{nullptr, refbranch};
            auto r2 = new lista{nullptr, refbranch};
            searchLists(searchBranchByName(db, "ALUNO"), r);
            printBranch(r);
            searchLists(searchBranchByName(db, "TURMA"), r2);
            printBranch(r2);

            //ADICIONANDO ALUNO
            mergeRefBranch(r2,r);
            printBranch(r2);

            //REMOVENDO ALUNO
            deleteLists(r2,r);
            printBranch(r2);

            break;
        }
        case 3:{

            auto r = new lista{nullptr, refbranch};
            searchLists(searchBranchByName(db, "CURSO"), r);
            printBranch(searchBranchByName(db, "CURSO"));
            deleteLists(searchBranchByName(db, "CURSO"), r);
            printBranch(searchBranchByName(db, "CURSO"));

            break;
        }

        case 4:{

            auto r = new lista{nullptr, refbranch};
            searchLists(searchBranchByName(db, "TURMA"), r);
            printBranch(r);
            break ;
        }
        case 5:{
            auto r3 = new lista{nullptr, refbranch};
            auto r2 = new lista{nullptr, refbranch};
            searchLists(searchBranchByName(db, "ALUNO"), r3);
            printBranch(r3);
            searchLists(searchBranchByName(db, "TURMA"), r2);
            printBranch(r2);

            //ADICIONANDO ALUNO
            mergeRefBranch(r2,r3);
            printBranch(r2);


           auto r = new lista{nullptr, refbranch};
           searchLists(searchBranchByName(db, "TURMA"), r);
            printBranch(r);
            break ;
        }

    }
*/


    std::string comando;
    auto s1 = new lista{nullptr, refbranch};
    auto s2 = new lista{nullptr, refbranch};


    while(std::cin >> comando){
        std::cout << std::endl;



        if(comando == "VIEW"){

            std::cin >> comando;
            if(comando == "S1") {
                printBranch(s1);
            } else if  (comando == "S2") {
                printBranch(s2);
            } else {
                printBranch(searchBranchByName(db, comando));
            }

        }
        else if(comando == "ADD"){

            std::cin >> comando;
            addLine(db, searchBranchByName(db, comando));

        }
        else if(comando == "DELETE"){
            std::cin >> comando;
            if(comando == "S1"){
                std::cin >> comando;
                if(comando == "S1"){
                    deleteLists(s1, s1);
                } else if (comando == "S2"){
                    deleteLists(s1, s2);
                } else {
                    std::cout << "Necessita ser S1 ou S2" << std::endl;
                }
            }
            else if(comando == "S2") {
                std::cin >> comando;
                if(comando == "S1"){
                    deleteLists(s2, s1);
                } else if (comando == "S2"){
                    deleteLists(s2, s2);
                } else {
                    std::cout << "Necessita ser S1 ou S2" << std::endl;
                }
            }
            else {
                lista * D = searchBranchByName(db, comando);
                if(D){
                    std::cin >> comando;
                    if(comando == "S1"){
                        deleteLists(D, s1);
                    }
                    else if(comando == "S2") {
                        deleteLists(D, s2);
                    } else {
                        std::cout << "Necessita ser S1 ou S2" << std::endl;
                    }

                }

            }

        }
        else if(comando == "SEARCH"){
            std::cin >> comando;
            if(comando == "S1"){

                std::cin >> comando;
                deleteRecursive(s1);
                s1 = new lista{nullptr, refbranch};
                searchLists(searchBranchByName(db, comando), s1);

            } else if ( comando == "S2"){

                std::cin >> comando;
                deleteRecursive(s2);
                s2 = new lista{nullptr, refbranch};
                searchLists(searchBranchByName(db, comando), s2);

            } else {

                std::cout << "A lista que recerá a busca deve ser S1 ou S2" << std::endl;

            }
        }
        else if(comando == "MERGE"){
            std::cin >> comando;
            if(comando == "S1"){
                std::cin >> comando;
                if(comando == "S2"){
                    //Merge
                    mergeRefBranch(s1,s2);
                } else if( comando == "S1"){
                    //Merge
                    mergeRefBranch(s1,s1);
                } else {
                    std::cout << "Necessita ser S1 ou S2" << std::endl;
                }
            }
            else if (comando == "S2"){
                std::cin >> comando;
                if(comando == "S2"){
                    //Merge
                    mergeRefBranch(s2,s2);
                } else if( comando == "S1"){
                    //Merge
                    mergeRefBranch(s2,s1);
                } else {
                    std::cout << "Necessita ser S1 ou S2" << std::endl;
                }
            }
            else {
                lista * D = searchBranchByName(db, comando);
                if(D){
                    std::cin >> comando;
                    if(comando == "S2"){
                        mergeRefBranch(D,s1);
                    } else if( comando == "S1"){
                        mergeRefBranch(D,s1);
                    } else {
                        std::cout << "Necessita ser S1 ou S2" << std::endl;
                    }
                }
            }
        }
        else if(comando == "SHOW"){
            listBranchs(db);
        }
        else if(comando == "CLEAR"){
            std::cin >> comando;
            if(comando == "S1"){
                deleteRecursive(s1);
                s1 = new lista {nullptr, refbranch};
            }
            else if(comando == "S2"){
                deleteRecursive(s2);
                s2 = new lista {nullptr, refbranch};
            } else {
                std::cout << "Necessita ser S1 ou S2" << std::endl;
            }
        }
        else if(comando == "EXIT"){


            deleteRecursive(s1);
            deleteRecursive(s2);
            deleteRecursive(db);


            return 0;
        }
        else {
            std::cout << "Comando não encontrado" << std::endl;
        }
    }


}