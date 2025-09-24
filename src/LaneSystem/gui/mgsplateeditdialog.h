#pragma once

#include <QStackedLayout>
#include <QStringListModel>
#include <QWidget>

class ElaText;
class ElaListView;
class MgsColorDelegate;
class MgsColorModel;
class MgsPlateEdit;
class MgsBaseDialog;
class MgsKeyDelegate;
class MgsPlateEditDialog : public QWidget
{
    Q_OBJECT
public:
    explicit MgsPlateEditDialog(QWidget *parent = nullptr);
    ~MgsPlateEditDialog() override;

    void setPlate(const QString &plate);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void initUi();
    void initKeyView(ElaListView *view, QStringListModel *model, int height, int spacing);

    int getNextRowForTab1(int currentRow, int key);

private:
    // 车牌显示区域
    MgsPlateEdit *m_plateEdit = nullptr;
    ElaText *m_plateEditIcon = nullptr;
    // 确认对话框
    MgsBaseDialog *m_contentDialog = nullptr;
    // 车牌颜色选择模型视图
    ElaListView *m_tab0 = nullptr;
    QStringListModel *m_model0 = nullptr;
    // 省份选择模式视图
    ElaListView *m_tab1 = nullptr;
    QStringListModel *m_model1 = nullptr;
    // 字母选择模型视图
    ElaListView *m_tab2 = nullptr;
    QStringListModel *m_model2 = nullptr;
    // 专用选择模式视图
    ElaListView *m_tab3 = nullptr;
    QStringListModel *m_model3 = nullptr;
    // 模型代理
    MgsColorDelegate *m_colorDelegate = nullptr;
    MgsKeyDelegate *m_keyDelegate = nullptr;
    // 堆叠布局
    QStackedLayout *m_tabLayout = nullptr;
    // 输入光标聚焦
    bool m_isFocusEdit = true;
    // 当前所选择内容索引
    QModelIndex m_curSelected;

    const QStringList m_colors = QStringList({"蓝", "黄", "绿", "拼", "黑", "白", "临", "渐"});
    const QStringList m_provinces = QStringList({"闽", "浙", "粤", "苏", "赣", "沪", "京", "皖", "豫", "湘", "蒙",
                                                 "辽", "吉", "黑", "津", "冀", "鲁", "鄂", "晋", "桂", "琼", "渝",
                                                 "川", "贵", "云", "藏", "陕", "甘", "青", "宁", "新"});
    const QStringList m_letters = QStringList({"A", "B", "C", "D", "E", "F", "G", "H", "J", "K", "L", "M",
                                               "N", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"});
    const QStringList m_specialWords = QStringList({"领", "使", "警", "学", "挂", "港", "澳", "试", "超", "应急", "*"});
};
