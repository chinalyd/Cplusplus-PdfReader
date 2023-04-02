#include "naviviewer.h"
#include<QProxyStyle>//style support
NaviViewer::NaviViewer()
{

}
TocTreeItem::TocTreeItem(QString des, TocTreeItem* parentItem){
    m_description = des;
    m_parentItem = parentItem;
    m_childList.clear();
    qDeleteAll(m_childList);
}
TocTreeItem::~TocTreeItem(){
    qDeleteAll(m_childList);
}
void TocTreeItem::appendChild(TocTreeItem *child){
    m_childList.append(child);
}
TocTreeItem* TocTreeItem::child(int row){
    return m_childList.value(row);
}
int TocTreeItem::childCount() const{
    return m_childList.count() > 0 ? m_childList.count() : 0;
}
int TocTreeItem::columnCount() const{
    return 1;
}
QString TocTreeItem::getDescription(int column) const{
    return m_description;
}
int TocTreeItem::row() const{
    if(m_parentItem)
        return m_parentItem->m_childList.indexOf(const_cast<TocTreeItem*>(this));
}
TocTreeItem* TocTreeItem::parentItem(){
    return m_parentItem;
}
TocTreeModel::TocTreeModel(QDomDocument* domdoc, QObject *parent){
    if(domdoc == NULL){
        return;
    }
    QDomNode domNode = domdoc->firstChild();
    rootItem = new TocTreeItem("domNode.toElement().tagName()");
    TocTreeItem* treeItem;
    for(QDomNode childNode = domNode; !childNode.isNull(); childNode = childNode.nextSibling()){
        QString strDom = childNode.toElement().tagName();
        treeItem = new TocTreeItem(strDom, rootItem);
        rootItem->appendChild(treeItem);
        parserDom(&domNode, treeItem);
    }
}
TocTreeModel::~TocTreeModel(){
    delete rootItem;
}
void TocTreeModel::parserDom(QDomNode* domNode, TocTreeItem* parentItem){
    TocTreeItem* treeItem = NULL;
    QString strDom;
    for(QDomNode childNode = domNode->firstChild(); !childNode.isNull(); childNode = childNode.nextSibling()){
        strDom = childNode.toElement().tagName();
        treeItem = new TocTreeItem(strDom, parentItem);
        parentItem->appendChild(treeItem);
        parserDom(&childNode, treeItem);
    }
}
int TocTreeModel::columnCount(const QModelIndex &parent) const{
    if(parent.isValid()){
        return static_vast<TocTreeItem*>(parent.internalPointer())->columnCount();
    }
    else
        return toorItem->columnCount();
}
int TocTreeModel::rowCount(const QModelIndex &parent) const{
    TocTreeItem *parentItem;
    if(parent.column() > 0)
        return 0;
    if(!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TocTreeItem*>(parent.internalPointer());
}
QVariant TocTreeModel::data(const QModelIndex *index, int role) const{
    if(!index->isValid())
        return QVariant();
    TocTreeitem *item = static_cast<TocTreeItem*>(index->internalPointer());
    return QVariant(item->getDescription(index->column()));
}
Qt::ItemFlags TocTreeModel::flags(const QModelIndex &index) const{
    if (!index.isValid()){
        return 0;
    }
    return QAbstractItemModel::flags(index);
}
QVariant TocTreeModel::heartData(int section, Qt::Orientation orientation, int role) const{
    return QVariant();
}
QModelIndex TocTreeModel::index(int row, int column, const QModelIndex &parent) const{
    if(!hasIndex(row, column, parent))
        return QModelIndex();
    TocTreeItem *parentItem;
    if(!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TocTreeItem*>(parent.internalPointer());
    TocTreeItem *childItem = parentItem->child(row);
    if(childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
QModelIndex TocTreeModel::parent(const QModelIndex &index) const{
    if(!index.isValid())
        return QModelIndex();
    TocTreeItem *childItem = static_cast<TocTreeItem*>(index.internalPointer());
    TocTreeItem* parentItem = childItem->parentItem();
    if(parentItem == rootItem)
        return QModelIndex();
    return createIndex(parentItem->row(), 0, parentItem);
}
NaviViewer::NaviViewer(IChildViewer* childviewer):m_IChildeViewer(childviewer), m_OutlineTree(NULL){
    InitUI();
    InitTOC();
    InitConnect();
}
void NaviViewer::InitUI(){
    //set layout
    m_OutlineWidget = new QWidget();
    m_OutlineLayout = new QVBoxLayout();
    m_OutlineWidget->setLayout(m_OutlineLayout);
    m_OutlineLayout->setContentsMargins(1, 1, 1, 1);
    //set label
    m_TumnbnailWidget = new ThumnbailWidget(m_IChildViewer);
    m_tabWidget = new QTabWidget();
    m_tabWidget->setTabPosition(QtabWidget::West);
    m_tabWidget->addTab(m_OutlineWidget, "Content");
    m_tabWidget->addTab(m_ThumbnailWidget, "Thumbnail");
    //vertical layout
    QVBoxLayout* naVLayout = new QVBoxLayout();
    naVLayout->addWidget(m_tabWidget);
    naVLayout->setContentsMargins(1, 1, 1, 1);
    this->setLayout(naVLayout);
}

