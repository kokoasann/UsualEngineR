#pragma once
struct CellBinary {

	Vector3		vertexPos[3];	//�Z���̒��_�̃|�W�V����
	int			linkCellNo[3];	//�Z���ɗאڂ��Ă���Z��
	Vector3		centerPos;		//�Z���̐^�񒆂̃|�W�V����
};
//�Z���N���X
struct Cell {
	Vector3		vertexPos[3];	//�Z���̒��_�̃|�W�V�����B
	Cell*		linkCells[3];	//�Z���ɗאڂ��Ă���Z���B
	Vector3		centerPos;		//�Z���̐^�񒆂̃|�W�V�����B
};

class Floor;
class Navimesh 
{
public:
	Navimesh();
	~Navimesh();
	void Init();
	const std::vector<Cell*>& GetCell() const
	{
		return m_cells;
	}
private:
	/// <summary>
	/// �ۑ����ꂽ�o�C�i���f�[�^����i�r���b�V�����\�z�B
	/// </summary>
	void BuildNavimeshFromBinary();
private:	
	std::vector<Cell*>	m_cells;						//�Z���̔z��B
};