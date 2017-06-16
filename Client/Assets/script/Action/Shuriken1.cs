using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Shuriken1 : MonoBehaviour {
    public GameObject firePrefab;
    public float speed = 10f;
    public float life = 3f;
    public float actionTime = 0.1f;
    public float coolTime = 1f;

    bool firing = false;
    float usedTimeAccum = 0f;

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    public void Run() {
        StartCoroutine("Fire1Coroutine");
    }

    IEnumerator Fire1Coroutine() {
        if (firing)
            yield break;

        firing = true;

        for (int i = 0; i < 3; ++i) {
            var fire = GameObject.Instantiate(firePrefab);
            var moveFire = fire.GetComponent<MoveFire>();
            moveFire.speed = speed;

            var effect = fire.GetComponent<effect>();
            effect.life = life;

            fire.transform.position = transform.position;
            fire.transform.rotation = transform.rotation;

            yield return new WaitForSeconds(actionTime);
        }

        firing = false;
    }
}
