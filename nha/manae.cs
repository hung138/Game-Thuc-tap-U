using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.EventSystems;

public class manae : MonoBehaviour {

	public allCa alca;
	public int solu;

	public string[] tenPla;

	public int baiRut;
	public int baiDanh;
	public int luot, huong;
	public int soT, mauT;
	public GameObject bai, bai2;

	public GameObject ShowDB;

	public bool toiCh;
	public int toiPl;

	Vector3 toiT;
	Quaternion toiR;

	public GameObject[] vt;
	public Sprite[] mauu;

	// chia bai luc dau
	public LayerMask mas;
	public bool khoiDau, hitBai;
	public int dem;
	public int check;
	public int maxDraw;

	public GameObject rever, cheEnd;
	public Sprite[] revSpi;
	public Color[] mauSac;

	public List<int> de1 = new List<int> ();
	public List<int> de2 = new List<int> ();
	public List<int> de3 = new List<int> ();
	public List<int> de4 = new List<int> ();

	public List<int> deckk = new List<int> ();

	void Start () {

	}

	void Update () {
		if (khoiDau) {
			if (!toiCh) {
				if (dem != 7 * 4) {
					ChiaBai (luot);
					if (huong == 1) {
						if (luot == 4) {
							luot = 1;
						} else {
							luot++;
						}
					} else {
						if (luot == 1) {
							luot = 4;
						} else {
							luot--;
						}
					}
					dem++;
				} else {
					luot = 0;
					khoiDau = false;
					ChiaBai (0);
					dem = 0;
				}
			}
		}

		if (hitBai) {
			if (!toiCh) {
				if (dem != maxDraw) {
					ChiaBai (luot);
					dem++;
				} else {
					hitBai = false;
					dem = 0;
					if (huong == 1) {
						if (luot == 4) {
							luot = 1;
						} else {
							luot++;
						}
					} else {
						if (luot == 1) {
							luot = 4;
						} else {
							luot--;
						}
					}
					RandomDanhBai ();
				}
			}
		}

		if (toiCh) {
			bai2.transform.position = Vector3.MoveTowards (bai2.transform.position, toiT, 20 * Time.deltaTime);
			bai2.transform.rotation = Quaternion.Slerp (bai2.transform.rotation, toiR, 30 * Time.deltaTime);

			if (bai2.transform.position.x == toiT.x && bai2.transform.position.y == toiT.y) {
				toiCh = false;

				if (toiPl == 0) {
					if (vt [0].transform.childCount >= 4) {
						Destroy (vt [0].transform.GetChild (3).gameObject);
					}
				}
					
				bai2.transform.SetParent (vt [toiPl].transform);

				bai2.AddComponent (typeof(infoCard));
				bai2.GetComponent <infoCard> ().stt = baiRut;
				bai2.GetComponent <infoCard> ().so = alca.soDem [baiRut];
				bai2.GetComponent <infoCard> ().mau = alca.mauu [baiRut];

				bai2 = null;

				if (toiPl == 0) {
					// check ket qua
					if (luot > 0 && vt [luot].transform.childCount == 1) {
						print ("Winner: Player " + luot);
						cheEnd.transform.GetChild (0).GetComponent <Text>().text = "" + vt[luot].transform.GetChild (0).GetChild (0).GetComponent <TextMesh>().text 
							+ " won";
						cheEnd.SetActive (true);
					} else {
						soT = alca.soDem [baiDanh];
						mauT = alca.mauu [baiDanh];
						if (mauT > 0) {
							vt [toiPl].transform.GetChild (2).GetComponent <SpriteRenderer> ().sprite = mauu [mauT - 1];

							if (alca.soDem [baiDanh] == 10) {  // skip
								ShowCardDB (alca.soDem [baiDanh]);
							} else if (alca.soDem [baiDanh] == 11) {  // reverse
								ShowCardDB (alca.soDem [baiDanh]);
							} else if (alca.soDem [baiDanh] == 12) {  // +2
								ShowCardDB (alca.soDem [baiDanh]);
							} else {
								if (luot == 0) {
									luot = 1;
								} else {
									if (huong == 1) {
										if (luot == 4) {
											luot = 1;
										} else {
											luot++;
										}
									} else {
										if (luot == 1) {
											luot = 4;
										} else {
											luot--;
										}
									}
								}

								RandomDanhBai ();
							}
						} else {
							// card wild
							if (alca.soDem [baiDanh] == 13) {  // chon mau
								ShowCardDB (alca.soDem [baiDanh]);
							} else if (alca.soDem [baiDanh] == 14) {  // chon mau, + 4
								ShowCardDB (alca.soDem [baiDanh]);
							}
						}
					}
				} else {
					if (!khoiDau && !hitBai) {
						RandomDanhBai ();
					}
				}
			}
		} else {

			// chon object
			if (/*Input.touchCount > 0*/ !EventSystem.current.IsPointerOverGameObject () 
				&& Input.GetMouseButtonDown (0) && luot == 1 && !khoiDau && !hitBai) {

				Vector2 worldPoint = Camera.main.ScreenToWorldPoint (Input.mousePosition);
				RaycastHit2D hit = Physics2D.Raycast (worldPoint, Vector2.zero, 100, mas);
				if (hit) {
					infoCard b = hit.collider.gameObject.GetComponent <infoCard> ();
					if (b.mau == 0 || (b.mau == mauT) || (b.so == alca.soDem [baiDanh])) {
						if (bai2 != null) {
							if (bai2 == hit.collider.gameObject) {
							/*	Vector3 vtM = bai2.transform.position;
								bai2.transform.position = new Vector3 (vtM.x, vtM.y - 0.5f, vtM.z);

								bai2 = null;*/
								DanhBai (0, 1);
							} else {
								int conSo = vt [1].transform.childCount;
								if (conSo > 1) {
									for (int i = 1; i < conSo; i++) {
										Vector3 vtM = vt [1].transform.GetChild (i).position;
										vt [1].transform.GetChild (i).position = new Vector3 (vtM.x, vt [1].transform.position.y, vtM.z);
									}
								}

								bai2 = hit.collider.gameObject;
								Vector3 vtM2 = bai2.transform.position;
								bai2.transform.position = new Vector3 (vtM2.x, vt [1].transform.position.y + 0.5f, vtM2.z);
							}
						} else {
							bai2 = hit.collider.gameObject;
							Vector3 vtM2 = bai2.transform.position;
							bai2.transform.position = new Vector3 (vtM2.x, vtM2.y + 0.5f, vtM2.z);
						}
					}
				} else {
					bai2 = null;
				}
			}
		}


	/*	if (Input.GetKeyDown (KeyCode.A)) {
			ChiaBai (0);
		}

		if (Input.GetKeyDown (KeyCode.Space)) {
			khoiDau = true;
		}*/
	}

	public void MixBai(){
		deckk.Clear ();

		List<int> he = new List<int> ();
		for (int i = 0; i < alca.cardName.Length; i++) {
			he.Add (i);
		}

		for (int i = 0; i < alca.cardName.Length; i++) {
			int ran = Random.Range (0, he.Count);
			int ch = he [ran];
			deckk.Add (ch);
			he.RemoveAt (ran);
		}
	}
		
	public void ChiaBai(int cho){
		toiR = Quaternion.Euler(0, 0, 0);
		if (cho == 1) {
			toiR = Quaternion.Euler (0, 0, 0);
		} else if (cho == 3) {
			toiR = Quaternion.Euler (0, 0, 180);
		} else if (cho == 2) {
			toiR = Quaternion.Euler (0, 0, 90);
		} else if (cho == 4) {
			toiR = Quaternion.Euler (0, 0, 270);
		}

		bai2 = (GameObject)Instantiate (bai, vt [0].transform.GetChild (0).position, bai.transform.rotation);

		if (cho > 0) {
			int soCon = vt [cho].transform.childCount;
			if (soCon > 1) {
				for (int i = 1; i < soCon; i++) {
					Vector3 vtM = vt [cho].transform.GetChild (i).position;
					if (cho == 1) {
						vt [cho].transform.GetChild (i).position = new Vector3 (vtM.x - 0.5f, vtM.y, vtM.z);
					} else if (cho == 3) {
						vt [cho].transform.GetChild (i).position = new Vector3 (vtM.x + 0.5f, vtM.y, vtM.z);
					} else if (cho == 2) {
						vt [cho].transform.GetChild (i).position = new Vector3 (vtM.x, vtM.y - 0.1f, vtM.z);
					} else if (cho == 4) {
						vt [cho].transform.GetChild (i).position = new Vector3 (vtM.x, vtM.y + 0.1f, vtM.z);
					}
				}

				Vector3 vtM2 = vt [cho].transform.GetChild (soCon - 1).position;
				if (cho == 1) {
					toiT = new Vector3 (vtM2.x + 1f, vtM2.y, vtM2.z - 0.001f);
				} else if (cho == 3) {
					toiT = new Vector3 (vtM2.x - 1f, vtM2.y, vtM2.z - 0.001f);
				} else if (cho == 2) {
					toiT = new Vector3 (vtM2.x, vtM2.y + 0.2f, vtM2.z - 0.001f);
				} else if (cho == 4) {
					toiT = new Vector3 (vtM2.x, vtM2.y - 0.2f, vtM2.z - 0.001f);
				}
			} else {
				toiT = vt [cho].transform.position;
			}
		} else {
			toiT = vt [cho].transform.GetChild (1).position;
		}

		baiRut = deckk [0];
		deckk.RemoveAt (0);
		if (cho == 0) {
			baiDanh = baiRut;
			bai2.GetComponent <SpriteRenderer> ().sprite = alca.cardSpi [baiRut];
		} else if (cho == 1) {
			de1.Add (baiRut);
			bai2.layer = 9;
			bai2.GetComponent <SpriteRenderer> ().sprite = alca.cardSpi [baiRut];
		} else if (cho == 2) {
			de2.Add (baiRut);
		} else if (cho == 3) {
			de3.Add (baiRut);
		} else if (cho == 4) {
			de4.Add (baiRut);
		}

		toiPl = cho;
		toiCh = true;

		rever.GetComponent <AudioSource> ().Play ();
	}

	public void RandomDanhBai(){
		for (int i = 1; i < 5; i++) {
			vt [i].transform.GetChild (0).GetChild (0).GetComponent <TextMesh> ().color = Color.white;
		}
		vt [luot].transform.GetChild (0).GetChild (0).GetComponent <TextMesh> ().color = Color.green;

		List<int> coThe = new List<int> ();
		coThe.Clear ();

		int soCon = vt [luot].transform.childCount;
		if (soCon > 1) {
			for (int i = 1; i < soCon; i++) {
				infoCard b = vt [luot].transform.GetChild (i).gameObject.GetComponent <infoCard> ();
				if (b.mau == 0 || (b.mau == mauT) || (b.so == alca.soDem [baiDanh])) {
					coThe.Add (i);
				}
			}
		}

		int demCoT = coThe.Count;
		if (demCoT > 0) {
			check = 0;

			//print (luot);
			if (luot != 1) {
				int ran = Random.Range (0, demCoT);

				bai2 = vt [luot].transform.GetChild (coThe [ran]).gameObject;
				DanhBai (1, luot);
			}
		} else {
			// rut bai
			check++;
			if (check == 2) {
				check = 0;

				if (luot == 0) {
					luot = 1;
				} else {
					if (huong == 1) {
						if (luot == 4) {
							luot = 1;
						} else {
							luot++;
						}
					} else {
						if (luot == 1) {
							luot = 4;
						} else {
							luot--;
						}
					}
				}
					
				RandomDanhBai ();
			} else {
				ChiaBai (luot);
			}
		}
	}

	public void DanhBai(float ddr, int ch){
		StartCoroutine (DanhRaBai (ddr, ch));
	}

	IEnumerator DanhRaBai(float dr, int cho){
		yield return new WaitForSeconds (dr);

		toiR = Quaternion.Euler(0, 0, 0);
		toiT = vt [0].transform.GetChild (1).position;

		deckk.Add (baiDanh);
		baiDanh = bai2.GetComponent <infoCard> ().stt;

		if (cho == 1) {
			de1.Remove (baiDanh);
			bai2.layer = 0;
		} else if (cho == 2) {
			de2.Remove (baiDanh);
			bai2.GetComponent <SpriteRenderer> ().sprite = alca.cardSpi [baiDanh];
		} else if (cho == 3) {
			de3.Remove (baiDanh);
			bai2.GetComponent <SpriteRenderer> ().sprite = alca.cardSpi [baiDanh];
		} else if (cho == 4) {
			de4.Remove (baiDanh);
			bai2.GetComponent <SpriteRenderer> ().sprite = alca.cardSpi [baiDanh];
		}

		toiPl = 0;
		toiCh = true;

		int soCon = vt [cho].transform.childCount;
		int namO = 0;
		if (soCon > 1) {
			for (int i = 1; i < soCon; i++) {
				if (baiDanh == vt [cho].transform.GetChild (i).GetComponent <infoCard> ().stt) {
					namO++;
				} else {
					if (namO > 0) {
						Vector3 vtM = vt [cho].transform.GetChild (i).position;
						if (cho == 1) {
							vt [cho].transform.GetChild (i).position = new Vector3 (vtM.x - 0.5f, vtM.y, vtM.z);
						} else if (cho == 3) {
							vt [cho].transform.GetChild (i).position = new Vector3 (vtM.x + 0.5f, vtM.y, vtM.z);
						} else if (cho == 2) {
							vt [cho].transform.GetChild (i).position = new Vector3 (vtM.x, vtM.y - 0.1f, vtM.z);
						} else if (cho == 4) {
							vt [cho].transform.GetChild (i).position = new Vector3 (vtM.x, vtM.y + 0.1f, vtM.z);
						}
					} else {
						Vector3 vtM = vt [cho].transform.GetChild (i).position;
						if (cho == 1) {
							vt [cho].transform.GetChild (i).position = new Vector3 (vtM.x + 0.5f, vtM.y, vtM.z);
						} else if (cho == 3) {
							vt [cho].transform.GetChild (i).position = new Vector3 (vtM.x - 0.5f, vtM.y, vtM.z);
						} else if (cho == 2) {
							vt [cho].transform.GetChild (i).position = new Vector3 (vtM.x, vtM.y + 0.1f, vtM.z);
						} else if (cho == 4) {
							vt [cho].transform.GetChild (i).position = new Vector3 (vtM.x, vtM.y - 0.1f, vtM.z);
						}
					}
				}
			}
				
		}
		rever.GetComponent <AudioSource> ().Play ();
	}

	public void ChonMau(int cho){
		if (luot == 1) {
			for (int i = 0; i < 4; i++) {
				if (i == cho) {
					ShowDB.transform.GetChild (3).GetChild (i).gameObject.SetActive (true);
				} else {
					ShowDB.transform.GetChild (3).GetChild (i).gameObject.SetActive (false);
				}
			}

			mauT = cho + 1;
			vt [toiPl].transform.GetChild (2).GetComponent <SpriteRenderer> ().sprite = mauu [cho];

			int soT = alca.soDem [baiDanh];
			if (soT == 13) {
				if (huong == 1) {
					if (luot == 4) {
						luot = 1;
					} else {
						luot++;
					}
				} else {
					if (luot == 1) {
						luot = 4;
					} else {
						luot--;
					}
				}

				StartCoroutine (AnShow2 ());
			} else if (soT == 14) {
				ShowCardDB (15);
			}
		}
	}

	IEnumerator AnShow2(){
		yield return new WaitForSeconds (1);
		ShowDB.SetActive (false);

		RandomDanhBai ();
	}

	public void ShowCardDB(int cho){
		for (int i = 0; i < 4; i++) {
			ShowDB.transform.GetChild (3).GetChild (i).gameObject.SetActive (true);
		}

		for(int i = 0; i < ShowDB.transform.childCount; i++){
			ShowDB.transform.GetChild (i).gameObject.SetActive (false);
		}
		ShowDB.SetActive (true);

		if (cho == 10) {  // skip
			ShowDB.transform.GetChild (0).gameObject.GetComponent <Image>().color = mauSac[mauT - 1];
			ShowDB.transform.GetChild (0).gameObject.SetActive (true);
			StartCoroutine (AnShowCard (cho));
		} else if (cho == 11) {  // reverse
			ShowDB.transform.GetChild (1).gameObject.GetComponent <Image>().color = mauSac[mauT - 1];
			ShowDB.transform.GetChild (1).gameObject.GetComponent <Image> ().sprite = revSpi [huong];
			rever.GetComponent <SpriteRenderer> ().sprite = revSpi [huong];

			ShowDB.transform.GetChild (1).gameObject.SetActive (true);
			StartCoroutine (AnShowCard (cho));
		} else if (cho == 12) {  // +2
			ShowDB.transform.GetChild (2).gameObject.GetComponent <Image>().color = mauSac[mauT - 1];
			ShowDB.transform.GetChild (2).gameObject.SetActive (true);
			StartCoroutine (AnShowCard (cho));
		} else if (cho == 13) {  // wild
			ShowDB.transform.GetChild (3).gameObject.SetActive (true);
			if (luot != 1) {
				StartCoroutine (AnShowCard (cho));
			}
		} else if (cho == 14) {  // wild +4
			ShowDB.transform.GetChild (3).gameObject.SetActive (true);
			if (luot != 1) {
				StartCoroutine (AnShowCard (cho));
			}
		} else if (cho == 15) {  // +4
			ShowDB.transform.GetChild (4).gameObject.GetComponent <Image>().color = mauSac[mauT - 1];
			ShowDB.transform.GetChild (4).gameObject.SetActive (true);
			StartCoroutine (AnShowCard (cho));
		}
	}

	IEnumerator AnShowCard(int cho){
		yield return new WaitForSeconds (1);

		if (cho == 10) {  // skip
			if (luot == 0) {
				if (huong == 1) {
					luot = 2;
				} else {
					luot = 4;
				}
			} else {
				if (huong == 1) {
					if (luot == 4) {
						luot = 2;
					} else if (luot == 3) {
						luot = 1;
					} else {
						luot += 2;
					}
				} else {
					if (luot == 1) {
						luot = 3;
					} else if (luot == 2) {
						luot = 4;
					} else {
						luot -= 2;
					}
				}
			}

			ShowDB.SetActive (false);

			yield return new WaitForSeconds (0.5f);
			RandomDanhBai ();
		} else if (cho == 11) {  // reverse
			if (luot == 0) {
				luot = 1;
				if (huong == 1) {
					huong = 0;
				} else {
					huong = 1;
				}
			} else {
				if (huong == 1) {
					huong = 0;
					if (luot == 1) {
						luot = 4;
					} else {
						luot--;
					}
				} else {
					huong = 1;
					if (luot == 4) {
						luot = 1;
					} else {
						luot++;
					}
				}
			}

			ShowDB.SetActive (false);

			yield return new WaitForSeconds (0.5f);
			RandomDanhBai ();
		} else if (cho == 12) {  // +2
			if (luot == 0) {
				luot = 1;
			} else {
				if (huong == 1) {
					if (luot == 4) {
						luot = 1;
					} else {
						luot++;
					}
				} else {
					if (luot == 1) {
						luot = 4;
					} else {
						luot--;
					}
				}
			}

			ShowDB.SetActive (false);

			yield return new WaitForSeconds (0.5f);
			maxDraw = 2;
			hitBai = true;
		} else if (cho == 13) {  // wild
			int ran = Random.Range (0, 4);
			for (int i = 0; i < 4; i++) {
				if (i == ran) {
					ShowDB.transform.GetChild (3).GetChild (i).gameObject.SetActive (true);
				} else {
					ShowDB.transform.GetChild (3).GetChild (i).gameObject.SetActive (false);
				}
			}
				
			mauT = ran + 1;
			vt [toiPl].transform.GetChild (2).GetComponent <SpriteRenderer> ().sprite = mauu [ran];

			if (luot == 0) {
				luot = 1;
			} else {
				if (huong == 1) {
					if (luot == 4) {
						luot = 1;
					} else {
						luot++;
					}
				} else {
					if (luot == 1) {
						luot = 4;
					} else {
						luot--;
					}
				}
			}

			yield return new WaitForSeconds (1);
			ShowDB.SetActive (false);
			RandomDanhBai ();
		} else if (cho == 14) {  // wild +4
			int ran = Random.Range (0, 4);
			for (int i = 0; i < 4; i++) {
				if (i == ran) {
					ShowDB.transform.GetChild (3).GetChild (i).gameObject.SetActive (true);
				} else {
					ShowDB.transform.GetChild (3).GetChild (i).gameObject.SetActive (false);
				}
			}

			mauT = ran + 1;
			vt [toiPl].transform.GetChild (2).GetComponent <SpriteRenderer> ().sprite = mauu [ran];

			yield return new WaitForSeconds (1);
			ShowCardDB (15);
		} else if (cho == 15) {  // +4
			if (luot == 0) {
				luot = 1;
			} else {
				if (huong == 1) {
					if (luot == 4) {
						luot = 1;
					} else {
						luot++;
					}
				} else {
					if (luot == 1) {
						luot = 4;
					} else {
						luot--;
					}
				}
			}

			ShowDB.SetActive (false);

			yield return new WaitForSeconds (0.5f);
			maxDraw = 4;
			hitBai = true;
		}
	}

	public void ResetBai(){
		for (int i = 0; i < vt.Length; i++) {
			if (i == 0) {
				if (vt [i].transform.childCount >= 4) {
					Destroy (vt [i].transform.GetChild (3).gameObject);
				}
			} else {
				int lengg = vt [i].transform.childCount;
				if (lengg > 1) {
					for (int j = 1; j < lengg; j++) {
						Destroy (vt [i].transform.GetChild (j).gameObject);
					}
				}
			}
		}
		huong = Random.Range (0, 2);
		if (huong == 0) {
			rever.GetComponent <SpriteRenderer> ().sprite = revSpi [1];
		} else {
			rever.GetComponent <SpriteRenderer> ().sprite = revSpi [0];
		}

		MixBai ();
		check = 0;
		baiDanh = -1;
		luot = 1;
		//huong = 1;

		StartCoroutine (batDauChia ());
	}

	IEnumerator batDauChia(){
		yield return new WaitForSeconds (1);
		khoiDau = true;
	}
}
